#include "kernel/yosys.h"
#include "kernel/rtlil.h"
#include "kernel/celltypes.h"

USING_YOSYS_NAMESPACE


struct WolfEmuSigSplicePass : public Pass {
    WolfEmuSigSplicePass() : Pass("wolf_emu_sig_splice", "wolf_emu 信号铰接处理流程") {}

    dict<RTLIL::SigBit, std::set<std::pair<RTLIL::SigSpec, int>>> sig_bit_map; // 用于存储信号映射
    dict<std::set<std::pair<RTLIL::SigSpec, int>>, RTLIL::SigSpec> wire_sig_bit_cluster; // 用于聚类
    dict<std::tuple<RTLIL::SigSpec, int, RTLIL::SigBit>, RTLIL::SigBit> sig_bit_update_map; // 用于存储信号与线的映射 <old_sig_bit, new_sig_bit>
    dict<RTLIL::SigBit, RTLIL::SigBit> input_sig_bit_update_map;
    dict<RTLIL::SigBit, RTLIL::SigBit> output_sig_bit_update_map; // 用于存储线与信号的映射 <new_sig_bit, old_sig_bit>

    void add_sig_spec_to_map(const RTLIL::SigSpec &sig_spec) {
        for (int chunk_index = 0; chunk_index < sig_spec.chunks().size(); chunk_index++) {
            auto &chunk = sig_spec.chunks()[chunk_index];
            if (!chunk.is_wire()) {
                // 如果不是 wire，直接跳过
                continue;
            }
            for(int i = 0; i < chunk.width; i++) {
                RTLIL::SigBit sig_bit = chunk[i];
                // 查找 sig_bit 是否已存在于 sig_bit_map 中
                if (sig_bit_map.find(sig_bit) == sig_bit_map.end()) {
                    // 如果不存在，则添加
                    sig_bit_map[sig_bit] = std::set<std::pair<RTLIL::SigSpec, int>>();
                }
                // 将当前的 SigSpec 和 SigChunk 添加到 sig_bit_map 中
                sig_bit_map[sig_bit].insert(std::make_pair(sig_spec, chunk_index));
            }
        }
    }


    void gen_wire_sig_bit_cluster()
    {
        // 遍历 sig_bit_map，将每个 SigBit 的所有 SigSpec 和 SigChunk 聚合到 wire_sig_bit_cluster 中
        for (const auto &entry : sig_bit_map) {
            RTLIL::SigBit sig_bit = entry.first;
            const auto &sig_cluster_info = entry.second;
            // 在 wire_sig_bit_cluster 中查找是否已存在相同的 SigSpec 和 SigChunk 组合
            if (wire_sig_bit_cluster.find(sig_cluster_info) == wire_sig_bit_cluster.end()) {
                // 如果不存在，则创建一个新的 SigSpec
                wire_sig_bit_cluster[sig_cluster_info] = RTLIL::SigSpec();
            }
            // 将当前 SigBit 添加到 wire_sig_bit_cluster 中的 SigSpec 中
            wire_sig_bit_cluster[sig_cluster_info].append(sig_bit);
        }
    }

    void create_new_wires(Module *module)
    {
        // 遍历 wire_sig_bit_cluster，将每个 SigSpec 和 SigChunk 聚合到新的 Wire 中
        for (auto &entry : wire_sig_bit_cluster) {
            auto &sig_cluster_info = entry.first;
            auto &tmp_sig_spec = entry.second;
            tmp_sig_spec.sort_and_unify();
            for(auto &tmp_wire_chunk : tmp_sig_spec.chunks()) {
                log_assert(tmp_wire_chunk.is_wire());
                // 生成的新 wire 的名称是 old_name_highbitidx_lowbitidx
                IdString new_wire_name = module->uniquify(tmp_wire_chunk.wire->name.str() + 
                std::string("_") + std::to_string(tmp_wire_chunk.offset+tmp_wire_chunk.width) + std::string("_") + std::to_string(tmp_wire_chunk.offset));
                Wire* new_wire_p = module->addWire(new_wire_name, tmp_wire_chunk.width);
                new_wire_p->attributes = tmp_wire_chunk.wire->attributes; // 复制属性
                // 添加用于调试的信息
                new_wire_p->set_string_attribute(ID("wolf_emu_old_wire_name"), tmp_wire_chunk.wire->name.str());
                new_wire_p->set_string_attribute(ID("wolf_emu_old_wire_offset"), std::to_string(tmp_wire_chunk.offset));
                // 更新 sig_bit_update_map
                for(int i = 0; i < tmp_wire_chunk.width; i++) {
                    RTLIL::SigBit old_sig_bit = tmp_wire_chunk[i];
                    RTLIL::SigBit new_sig_bit = RTLIL::SigBit(new_wire_p, i);
                    for(const auto & old_sig_spec_info : sig_cluster_info) {
                        sig_bit_update_map[std::make_tuple(old_sig_spec_info.first, old_sig_spec_info.second, old_sig_bit)] = new_sig_bit;
                        // 特别记录一下输入和输出信号位的映射
                        if(old_sig_bit.wire->port_input) {
                            input_sig_bit_update_map[old_sig_bit] = new_sig_bit;
                        } else if(old_sig_bit.wire->port_output) {
                            output_sig_bit_update_map[new_sig_bit] = old_sig_bit;
                        }
                    }
                }
            }
        }
    }

    // 更新一个 SigSpec，将其内部的 SigBit 替换为新的 SigBit
    SigSpec update_sig_spec(const SigSpec& old_sig_spec)
    {
        SigSpec new_sig_spec = old_sig_spec;
        for (int chunk_index = 0; chunk_index < old_sig_spec.chunks().size(); chunk_index++) {
            auto &chunk = old_sig_spec.chunks()[chunk_index];
            if (!chunk.is_wire()) {
                // 如果不是 wire，直接跳过
                continue;
            }
            for (int i = 0; i < chunk.width; i++) {
                RTLIL::SigBit old_sig_bit = chunk[i];
                // 查找 sig_bit_update_map 中是否有对应的映射
                auto it = sig_bit_update_map.find(std::make_tuple(old_sig_spec, chunk_index, old_sig_bit));
                if (it != sig_bit_update_map.end()) {
                    // 如果存在映射，则更新 SigSpec
                    new_sig_spec.replace(old_sig_bit, it->second);
                }
            }
        }
        return new_sig_spec;
    }

    void update_all_sigspecs(Module *module)
    {
        // 遍历所有的 connections，将其 SigSpec 更新为新的 SigSpec
        for (auto &sigsig : module->connections()) {
            RTLIL::SigSpec new_first = update_sig_spec(sigsig.first);
            RTLIL::SigSpec new_second = update_sig_spec(sigsig.second);
            // 添加新的 connections，旧的不用删除，留给 clean pass 处理
            module->connect(new_first, new_second);
        }
        // 遍历所有的 cells，将其 port 上的 SigSpec 更新为新的 SigSpec，重新连接到端口上
        for (auto* cell : module->cells()){
            auto port_connections = cell->connections();
            for(auto& entry: port_connections) {
                IdString port_name = entry.first;
                RTLIL::SigSpec old_sig_spec = entry.second;
                RTLIL::SigSpec new_sig_spec = update_sig_spec(old_sig_spec);
                cell->unsetPort(port_name); // 先取消旧的连接
                // 重新设置新的 SigSpec 到端口上
                cell->setPort(port_name, new_sig_spec);
            }
        }
    }

    void process_input(Module *module)
    {
        // 遍历所有的输入信号，将其 SigBit 更新为新的 SigBit 
        SigSpec input_sig_spec = SigSpec();
        for (auto &entry : input_sig_bit_update_map) {
            RTLIL::SigBit old_sig_bit = entry.first;
            input_sig_spec.append(old_sig_bit);
        }
        input_sig_spec.sort_and_unify();
        SigSpec new_input_sig_spec = SigSpec();
        for (auto &s : input_sig_spec.bits()) {
            new_input_sig_spec.append(input_sig_bit_update_map[s]);
        }
        // 创建一个新的 Cell，将两端连接起来
        auto *new_cell = module->addCell(NEW_ID, "$__WOLF_EMU_INPUT_SPLICE_");
        new_cell->setPort("I", input_sig_spec);
        new_cell->setPort("O", new_input_sig_spec);
    }

    void process_output(Module *module)
    {
        // 遍历所有的输出信号，将其 SigBit 更新为新的 SigBit 
        SigSpec output_sig_spec = SigSpec();
        for (auto &entry : output_sig_bit_update_map) {
            RTLIL::SigBit old_sig_bit = entry.first;
            output_sig_spec.append(old_sig_bit);
        }
        output_sig_spec.sort_and_unify();
        SigSpec new_output_sig_spec = SigSpec();
        for (auto &s : output_sig_spec.bits()) {
            new_output_sig_spec.append(output_sig_bit_update_map[s]);
        }
        // 创建一个新的 Cell，将两端连接起来
        auto *new_cell = module->addCell(NEW_ID, "$__WOLF_EMU_OUTPUT_SPLICE_");
        new_cell->setPort("I", new_output_sig_spec);
        new_cell->setPort("O", output_sig_spec);
    }
    

    void execute(std::vector<std::string> args, RTLIL::Design *design) override
    {
        (void)args;// 不需要额外参数
        // 只处理顶层模块
        auto m  = design->top_module();
        if (m == nullptr) {
            log("No top module found in the design.\n");
            return;
        }
        log("Processing module: %s\n", m->name.c_str());
        // 遍历所有 conns，将 SigSpec 添加到 sig_bit_map 中
        for (auto &sigsig : m->connections()) {
            RTLIL::SigSpec first_sig_spec = sigsig.first;
            RTLIL::SigSpec second_sig_spec = sigsig.second;
            add_sig_spec_to_map(first_sig_spec);
            add_sig_spec_to_map(second_sig_spec);
        }
        // 遍历所有 cells，将 SigSpec 添加到 sig_bit_map 中
        for (auto* cell : m->cells()) {
            for (auto &entry : cell->connections()) {
                RTLIL::SigSpec sig_spec = entry.second;
                add_sig_spec_to_map(sig_spec);
            }
        }
        // 生成 wire_sig_bit_cluster
        gen_wire_sig_bit_cluster();
        // 创建新的 wires
        create_new_wires(m);
        // 更新所有的 SigSpec
        update_all_sigspecs(m);
        // 处理输入信号
        process_input(m);
        // 处理输出信号
        process_output(m);
        log("Finished processing module: %s\n", m->name.c_str());
    }
} WolfEmuSigSplicePass;