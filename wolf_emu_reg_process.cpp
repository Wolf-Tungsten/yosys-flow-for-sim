#include "kernel/yosys.h"
#include "kernel/rtlil.h"

USING_YOSYS_NAMESPACE

struct WolfEMUDffPass : public Pass {
    WolfEMUDffPass() : Pass("wolf_emu_dff", "wolf_emu 处理寄存器") {}

    /* 映射表：旧类型 -> 新类型 */
    const dict<IdString, IdString> type_map = {
        {ID($dff),    ID($__WOLF_EMU_DFF_)},
        {ID($adff),   ID($__WOLF_EMU_ADFF_)},
        {ID($adffe),  ID($__WOLF_EMU_ADFFE_)},
        {ID($sdff),   ID($__WOLF_EMU_SDFF_)},
        {ID($sdffe),  ID($__WOLF_EMU_SDFFE_)}
    };

    void execute(std::vector<std::string> args, RTLIL::Design *design) override
    {
        (void)args;               // 本例不需要额外参数
        for (auto m : design->modules()) {
            if (m->get_blackbox_attribute()) continue;

            /* 先收集需要处理的 cell，避免边遍历边改名 */
            std::vector<RTLIL::Cell*> candidate_reg_vec;
            for (auto c : m->cells()) {
                if (type_map.count(c->type))
                    candidate_reg_vec.push_back(c);
                else if (c->type.in(ID($dlatch), ID($sr))) {
                    log_error("Unsupported register type %s in cell %s.%s\n",
                              log_id(c->type), log_id(m), log_id(c));
                }
            }

            for (auto c : candidate_reg_vec) {
                /* 找 Q 口连接的 wire */
                RTLIL::Wire *qw = nullptr;
                for (const auto &bit : c->getPort(ID::Q))
                    if (bit.is_wire()) { qw = bit.wire; break; }

                if (!qw) continue;

                std::string reg_name = log_id(qw->name);
                std::string new_reg_name = "\\WOLF_REG$" + reg_name;
                std::string new_wire_name = "\\" + reg_name + ".Q";

                //m->rename(qw, new_wire_name);
                m->rename(c, new_reg_name);
                //m->rename(c, m->uniquify(reg_name));
                //log("rename to %s",log_id(m->uniquify(std::string("\\") + reg_name)));
                /* 替换类型并打 keep 标记 */
                c->set_string_attribute(ID(zzj_reg_name), reg_name);
                c->type = type_map.at(c->type);
                c->set_bool_attribute(ID::keep);
            }
        }
    }
} WolfEMUDffPass;