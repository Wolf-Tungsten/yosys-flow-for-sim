#include "kernel/yosys.h"
#include "kernel/rtlil.h"
#include "kernel/celltypes.h"

USING_YOSYS_NAMESPACE


struct WolfEmuSigSplicePass : public Pass {
    WolfEmuSigSplicePass() : Pass("wolf_emu_sig_splice", "wolf_emu 信号铰接处理流程") {}

    dict<RTLIL::SigBit, std::set<std::pair<RTLIL::SigSpec, int>>> sig_bit_map; // 用于存储信号映射
    dict<std::set<std::pair<RTLIL::SigSpec, int>>, RTLIL::SigSpec> sig_bit_map_reverse; // 用于聚类
    dict<std::tuple<RTLIL::SigSpec, int, RTLIL::SigBit>, RTLIL::SigBit> sig_bit_update_map; // 用于存储信号与线的映射 <old_sig_bit, new_sig_bit>

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


    void gen_sig_bit_map_reverse()
    {
        // 遍历 sig_bit_map，将每个 SigBit 的所有 SigSpec 和 SigChunk 聚合到 sig_bit_map_reverse 中
        for (const auto &entry : sig_bit_map) {
            RTLIL::SigBit sig_bit = entry.first;
            const auto &sig_cluster_info = entry.second;
            // 在 sig_bit_map_reverse 中查找是否已存在相同的 SigSpec 和 SigChunk 组合
            if (sig_bit_map_reverse.find(sig_cluster_info) == sig_bit_map_reverse.end()) {
                // 如果不存在，则创建一个新的 SigSpec
                sig_bit_map_reverse[sig_cluster_info] = RTLIL::SigSpec();
            }
            // 将当前 SigBit 添加到 sig_bit_map_reverse 中的 SigSpec 中
            sig_bit_map_reverse[sig_cluster_info].append(sig_bit);
        }
    }

    void create_new_wires(Module *module)
    {
        // 遍历 sig_bit_map_reverse，将每个 SigSpec 和 SigChunk 聚合到新的 Wire 中
        for (auto &entry : sig_bit_map_reverse) {
            auto &sig_cluster_info = entry.first;
            auto &tmp_sig_spec = entry.second;
            tmp_sig_spec.sort_and_unify();
            for(auto &tmp_wire_chunk : tmp_sig_spec.chunks()) {
                log_assert(tmp_wire_chunk.is_wire());
                IdString new_wire_name = module->uniquify(tmp_wire_chunk.wire->name.str() + 
                std::string("_") + std::to_string(tmp_wire_chunk.offset));
                Wire* new_wire_p = module->addWire(new_wire_name, tmp_wire_chunk.width);
                new_wire_p->attributes = tmp_wire_chunk.wire->attributes;
                new_wire_p->set_string_attribute(ID("wolf_emu_old_wire"), tmp_wire_chunk.wire->name.str());
                new_wire_p->set_string_attribute(ID("wolf_emu_old_wire_offset"), std::to_string(tmp_wire_chunk.offset));
                // 更新 sig_bit_update_map
                for(int i = 0; i < tmp_wire_chunk.width; i++) {
                    RTLIL::SigBit old_sig_bit = tmp_wire_chunk[i];
                    RTLIL::SigBit new_sig_bit = RTLIL::SigBit(new_wire_p, i);
                    for(const auto & old_sig_spec_info : sig_cluster_info) {
                        sig_bit_update_map[std::make_tuple(old_sig_spec_info.first, old_sig_spec_info.second, old_sig_bit)] = new_sig_bit;
                    }
                }
            }
        }
    }

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

    // TODO

    void execute(std::vector<std::string> args, RTLIL::Design *design) override
    {
        (void)args;// 不需要额外参数
        // 只处理顶层模块
        auto m  = design->top_module();
        if (m == nullptr) {
            log("No top module found in the design.\n");
            return;
        }
        // 遍历设计中所有的 connections，将所有 SigSpec 添加到 sig_bit_map 中
        for (const auto &sigsig : m->connections()) {
            add_sig_spec_to_map(sigsig.first);
            add_sig_spec_to_map(sigsig.second);
        }
        for (const auto &cell : m->cells()) {
            for (const auto &port : cell->connections()) {
                RTLIL::SigSpec sig_spec = port.second;
                add_sig_spec_to_map(sig_spec);
            }
        }
        // 聚类，生成 sig_bit_map_reverse
        gen_sig_bit_map_reverse();
    }
} WolfEmuSigSplicePass;