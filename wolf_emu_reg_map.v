module \$adff (CLK, ARST, D, Q);

    parameter WIDTH = 0;
    parameter CLK_POLARITY = 1'b1;
    parameter ARST_POLARITY = 1'b1;
    parameter ARST_VALUE = 0;

    input CLK, ARST;
    input [WIDTH-1:0] D;
    output reg [WIDTH-1:0] Q;

    $__WOLF_ADFF_ #(
        .WIDTH(WIDTH),
        .CLK_POLARITY(CLK_POLARITY),
        .ARST_POLARITY(ARST_POLARITY),
        .ARST_VALUE(ARST_VALUE)
    ) _TECHMAP_REPLACE_ (
        .CLK(CLK),
        .ARST(ARST),
        .D(D),
        .Q(Q)
    );

endmodule

module \$adffe (CLK, ARST, EN, D, Q);

    parameter WIDTH = 0;
    parameter CLK_POLARITY = 1'b1;
    parameter EN_POLARITY = 1'b1;
    parameter ARST_POLARITY = 1'b1;
    parameter ARST_VALUE = 0;

    input CLK, ARST, EN;
    input [WIDTH-1:0] D;
    output reg [WIDTH-1:0] Q;
    
    $__WOLF_ADFFE_ #(
        .WIDTH(WIDTH),
        .CLK_POLARITY(CLK_POLARITY),
        .EN_POLARITY(EN_POLARITY),
        .ARST_POLARITY(ARST_POLARITY),
        .ARST_VALUE(ARST_VALUE)
    ) _TECHMAP_REPLACE_ (
        .CLK(CLK),
        .ARST(ARST),
        .EN(EN),
        .D(D),
        .Q(Q)
    );

endmodule

module \$adlatch (EN, ARST, D, Q);

    parameter WIDTH = 0;
    parameter EN_POLARITY = 1'b1;
    parameter ARST_POLARITY = 1'b1;
    parameter ARST_VALUE = 0;

    input EN, ARST;
    input [WIDTH-1:0] D;
    output reg [WIDTH-1:0] Q;

    $__WOLF_UNSUPPORT_REG_  _TECHMAP_REPLACE_(.D(D), .Q(Q));

endmodule

module \$aldff (CLK, ALOAD, AD, D, Q);

    parameter WIDTH = 0;
    parameter CLK_POLARITY = 1'b1;
    parameter ALOAD_POLARITY = 1'b1;

    input CLK, ALOAD;
    input [WIDTH-1:0] AD;
    input [WIDTH-1:0] D;
    output reg [WIDTH-1:0] Q;

    $__WOLF_UNSUPPORT_REG_  _TECHMAP_REPLACE_(.D(D), .Q(Q));

endmodule

module \$aldffe (CLK, ALOAD, AD, EN, D, Q);

    parameter WIDTH = 0;
    parameter CLK_POLARITY = 1'b1;
    parameter EN_POLARITY = 1'b1;
    parameter ALOAD_POLARITY = 1'b1;

    input CLK, ALOAD, EN;
    input [WIDTH-1:0] D;
    input [WIDTH-1:0] AD;
    output reg [WIDTH-1:0] Q;

    $__WOLF_UNSUPPORT_REG_  _TECHMAP_REPLACE_(.D(D), .Q(Q));

endmodule

module \$dff (CLK, D, Q);

    parameter WIDTH = 0;
    parameter CLK_POLARITY = 1'b1;

    input CLK;
    input [WIDTH-1:0] D;
    output reg [WIDTH-1:0] Q;

    $__WOLF_DFF_ #(
        .WIDTH(WIDTH),
        .CLK_POLARITY(CLK_POLARITY)
    ) _TECHMAP_REPLACE_ (
        .CLK(CLK),
        .D(D),
        .Q(Q)
    );

endmodule

module \$dffe (CLK, EN, D, Q);

    parameter WIDTH = 0;
    parameter CLK_POLARITY = 1'b1;
    parameter EN_POLARITY = 1'b1;

    input CLK, EN;
    input [WIDTH-1:0] D;
    output reg [WIDTH-1:0] Q;
    
    $__WOLF_DFFE_ #(
        .WIDTH(WIDTH),
        .CLK_POLARITY(CLK_POLARITY),
        .EN_POLARITY(EN_POLARITY)
    ) _TECHMAP_REPLACE_ (
        .CLK(CLK),
        .EN(EN),
        .D(D),
        .Q(Q)
    );

endmodule

module \$dffsr (CLK, SET, CLR, D, Q);

    parameter WIDTH = 0;
    parameter CLK_POLARITY = 1'b1;
    parameter SET_POLARITY = 1'b1;
    parameter CLR_POLARITY = 1'b1;

    input CLK;
    input [WIDTH-1:0] SET, CLR, D;
    output reg [WIDTH-1:0] Q;

    $__WOLF_UNSUPPORT_REG_  _TECHMAP_REPLACE_(.D(D), .Q(Q)); 

endmodule

module \$dffsre (CLK, SET, CLR, EN, D, Q);

    parameter WIDTH = 0;
    parameter CLK_POLARITY = 1'b1;
    parameter SET_POLARITY = 1'b1;
    parameter CLR_POLARITY = 1'b1;
    parameter EN_POLARITY = 1'b1;

    input CLK, EN;
    input [WIDTH-1:0] SET, CLR, D;
    output reg [WIDTH-1:0] Q;

    $__WOLF_UNSUPPORT_REG_  _TECHMAP_REPLACE_(.D(D), .Q(Q)); 

endmodule

module \$dlatch (EN, D, Q);

    parameter WIDTH = 0;
    parameter EN_POLARITY = 1'b1;

    input EN;
    input [WIDTH-1:0] D;
    output reg [WIDTH-1:0] Q;

    $__WOLF_UNSUPPORT_REG_  _TECHMAP_REPLACE_(.D(D), .Q(Q));  

endmodule

module \$dlatchsr (EN, SET, CLR, D, Q);

    parameter WIDTH = 0;
    parameter EN_POLARITY = 1'b1;
    parameter SET_POLARITY = 1'b1;
    parameter CLR_POLARITY = 1'b1;

    input EN;
    input [WIDTH-1:0] SET, CLR, D;
    output reg [WIDTH-1:0] Q;

    $__WOLF_UNSUPPORT_REG_  _TECHMAP_REPLACE_(.D(D), .Q(Q));  

endmodule

module \$sdff (CLK, SRST, D, Q);

    parameter WIDTH = 0;
    parameter CLK_POLARITY = 1'b1;
    parameter SRST_POLARITY = 1'b1;
    parameter SRST_VALUE = 0;

    input CLK, SRST;
    input [WIDTH-1:0] D;
    output reg [WIDTH-1:0] Q;
    
    $__WOLF_SDFF_ #(
        .WIDTH(WIDTH),
        .CLK_POLARITY(CLK_POLARITY),
        .SRST_POLARITY(SRST_POLARITY),
        .SRST_VALUE(SRST_VALUE)
    ) _TECHMAP_REPLACE_ (
        .CLK(CLK),
        .SRST(SRST),
        .D(D),
        .Q(Q)
    );

endmodule

module \$sdffce (CLK, SRST, EN, D, Q);

    parameter WIDTH = 0;
    parameter CLK_POLARITY = 1'b1;
    parameter EN_POLARITY = 1'b1;
    parameter SRST_POLARITY = 1'b1;
    parameter SRST_VALUE = 0;

    input CLK, SRST, EN;
    input [WIDTH-1:0] D;
    output reg [WIDTH-1:0] Q;
    
    $__WOLF_UNSUPPORT_REG_  _TECHMAP_REPLACE_(.D(D), .Q(Q)); 

endmodule

module \$sdffe (CLK, SRST, EN, D, Q);

    parameter WIDTH = 0;
    parameter CLK_POLARITY = 1'b1;
    parameter EN_POLARITY = 1'b1;
    parameter SRST_POLARITY = 1'b1;
    parameter SRST_VALUE = 0;

    input CLK, SRST, EN;
    input [WIDTH-1:0] D;
    output reg [WIDTH-1:0] Q;
    
    $__WOLF_SDFFE_ #(
        .WIDTH(WIDTH),
        .CLK_POLARITY(CLK_POLARITY),
        .EN_POLARITY(EN_POLARITY),
        .SRST_POLARITY(SRST_POLARITY),
        .SRST_VALUE(SRST_VALUE)
    ) _TECHMAP_REPLACE_ (
        .CLK(CLK),
        .SRST(SRST),
        .EN(EN),
        .D(D),
        .Q(Q)
    );

endmodule

module \$sr (SET, CLR, Q);

    parameter WIDTH = 0;
    parameter SET_POLARITY = 1'b1;
    parameter CLR_POLARITY = 1'b1;

    input [WIDTH-1:0] SET, CLR;
    output reg [WIDTH-1:0] Q;

   $__WOLF_UNSUPPORT_REG_  _TECHMAP_REPLACE_(.Q(Q));  

endmodule
