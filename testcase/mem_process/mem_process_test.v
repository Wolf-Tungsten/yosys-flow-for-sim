module memory_process_test #(
    parameter DATA_WIDTH = 64,    // 数据位宽
    parameter ADDR_WIDTH = 10,    // 地址位宽
    parameter MEM_DEPTH  = 2**ADDR_WIDTH  // 存储器深度
)(
    // 写端口 (同步)
    input  wire                     wclk,    // 写时钟
    input  wire                     w_en,    // 写使能
    input  wire [ADDR_WIDTH-1:0]    w_addr,  // 写地址
    input  wire [DATA_WIDTH-1:0]    w_data,  // 写数据
    
    // 同步读端口 (时钟边沿触发)
    input  wire                     rclk,    // 读时钟
    input  wire [ADDR_WIDTH-1:0]    r_addr1, // 同步读地址
    output reg  [DATA_WIDTH-1:0]    r_data1, // 同步读数据
    
    // 异步读端口 (组合逻辑)
    input  wire [ADDR_WIDTH-1:0]    r_addr2, // 异步读地址
    output wire [DATA_WIDTH-1:0]    r_data2,  // 异步读数据

    // 异步读口口不需要时钟信号
    input wire [ADDR_WIDTH-1:0] r_addr3, // 异步读地址
    output wire [DATA_WIDTH-1:0] r_data3  // 异步读数据

);

    // 存储器声明
    reg [DATA_WIDTH-1:0] mem [0:MEM_DEPTH-1];
    reg [DATA_WIDTH-1:0] mem_no_en [0:MEM_DEPTH-1]; // 用于异步读端口的存储器
    
    // 异步读端口 (组合逻辑)
    assign r_data2 = mem[r_addr2];
    assign r_data3 = mem_no_en[r_addr3];
    
    // 同步写操作
    always @(posedge wclk) begin
        if (w_en) begin
            mem[w_addr] <= w_data;
        end
    end

    always @(posedge wclk) begin
        // 同步写操作到异步读端口的存储器
        mem_no_en[w_addr] <= w_data;
    end
    
    // 同步读操作
    always @(posedge rclk) begin
        r_data1 <= mem[r_addr1];
    end

endmodule
 