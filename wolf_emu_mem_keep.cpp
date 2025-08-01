// 文件名: my_keep_pass.cc
#include "kernel/yosys.h"
#include "kernel/rtlil.h"

USING_YOSYS_NAMESPACE

struct WolfEmuKeepMemPass : public Pass {
    WolfEmuKeepMemPass() : Pass("wolf_emu_keep_mem", "Mark $mem_v2 cells to prevent further mapping") {}

    void execute(vector<string> args, RTLIL::Design *design) override {
        (void) args; // 忽略参数
        auto module = design->top_module();
        for (auto cell : module->cells()) {
            // 示例：匹配名称包含 "my_cell" 的 cell
             // 确保类型是 $mem_v2
            if (cell->type == "$mem_v2") {
                cell->type = "$__wolf_emu_mem_v2";
                log("Marked cell %s as keep.\n", log_id(cell->name));
            }
        }
    }

} WolfEmuKeepMemPass;

struct WolfEmuLetMemGoPass : public Pass {
    WolfEmuLetMemGoPass() : Pass("wolf_emu_let_mem_go", "Remove $mem_v2 cells from keep") {}

    void execute(vector<string> args, RTLIL::Design *design) override {
        (void) args; // 忽略参数
        auto module = design->top_module();
        for (auto cell : module->cells()) {
            // 示例：匹配名称包含 "my_cell" 的 cell
            if (cell->type == "$__wolf_emu_mem_v2") {
                cell->type = "$mem_v2";
                log("Removed keep attributes from cell %s.\n", log_id(cell->name));
            }
        }
    }

} WolfEmuLetMemGoPass;