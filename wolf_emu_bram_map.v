module \$__WOLF_MEM_64_WMASK_ (
    input wire CLK1, // 时钟信号
    input wire [16:0] A1ADDR, // 写地址
    input wire [63:0] A1DATA, // 写数据
    input wire [63:0] A1EN, // 写使能
    input wire [16:0] B1ADDR, // 读地址1
    output wire [63:0] B1DATA, // 读数据1
    input wire [16:0] B2ADDR, // 读地址2
    output wire [63:0] B2DATA  // 读数据2
);

    \$__wolf_emu_mem_mask #(
        .DATA_WIDTH(64),
        .ADDR_WIDTH(10)
    ) __TECHMAP_REPLACE (
        .wclk(CLK1),
        .w_addr(A1ADDR[9:0]),
        .w_data(A1DATA),
        .w_en(A1EN),
        .r_addr1(B1ADDR[9:0]),
        .r_data1(B1DATA),
        .r_addr2(B2ADDR[9:0]),
        .r_data2(B2DATA)
    );


endmodule

module  $__wolf_emu_mem_mask #(
    parameter DATA_WIDTH = 64,    // 数据位宽
    parameter ADDR_WIDTH = 10,    // 地址位宽
) (
    input wire wclk, // 写时钟
    input wire [ADDR_WIDTH-1:0] w_addr, // 写地址
    input wire [DATA_WIDTH-1:0] w_data, // 写数据
    input wire [DATA_WIDTH-1:0] w_en, // 写使能
    input wire [ADDR_WIDTH-1:0] r_addr1, // 异步读地址1
    output wire [DATA_WIDTH-1:0] r_data1, // 异步读数据1
    input wire [ADDR_WIDTH-1:0] r_addr2, // 异步读地址2
    output wire [DATA_WIDTH-1:0] r_data2  // 异步读数据2
);
    parameter MEM_DEPTH  = 2**ADDR_WIDTH;  // 存储器深度
    reg [DATA_WIDTH-1:0] mem [0:MEM_DEPTH-1]; // 存储器声明
    // 异步读端口 (组合逻辑)
    assign r_data1 = mem[r_addr1];
    assign r_data2 = mem[r_addr2];
    // 同步写操作带使能
    integer i;
    always @(posedge wclk) begin
        for(i = 0; i < DATA_WIDTH; i = i + 1) begin
            if (w_en[i]) begin
                mem[w_addr][i] <= w_data[i];
            end
        end
    end
endmodule


/*
  cell $__WOLF_MEM_64_WMASK_ \mem.0.0.0
    connect \A1ADDR { 7'0000000 $0$memwr$\mem$mem_process_test.v:31$1_ADDR[9:0]$4 }
    connect \A1DATA $0$memwr$\mem$mem_process_test.v:31$1_DATA[63:0]$5
    connect \A1EN $0$memwr$\mem$mem_process_test.v:31$1_EN[63:0]$6
    connect \B1ADDR { 7'0000000 \r_addr1 }
    connect \B1DATA $\mem$rdreg[0]$d
    connect \B2ADDR { 7'0000000 \r_addr2 }
    connect \B2DATA \r_data2
    connect \CLK1 \wclk
  end
*/