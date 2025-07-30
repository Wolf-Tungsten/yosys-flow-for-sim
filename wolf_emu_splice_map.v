module \$concat (A, B, Y);

    parameter A_WIDTH = 0;
    parameter B_WIDTH = 0;

    input [A_WIDTH-1:0] A;
    input [B_WIDTH-1:0] B;
    output [A_WIDTH+B_WIDTH-1:0] Y;

    $__WOLF_CONCAT_ #(
        .A_WIDTH(A_WIDTH),
        .B_WIDTH(B_WIDTH)
    ) _TECHMAP_REPLACE_ (
        .A(A),
        .B(B),
        .Y(Y)
    );

endmodule

(* keep *)
module $__WOLF_CONCAT_ (A, B, Y);

    parameter A_WIDTH = 0;
    parameter B_WIDTH = 0;

    input [A_WIDTH-1:0] A;
    input [B_WIDTH-1:0] B;
    output [A_WIDTH+B_WIDTH-1:0] Y;

    assign Y = {B, A};

endmodule


module \$slice (A, Y);

    parameter OFFSET = 0;
    parameter A_WIDTH = 0;
    parameter Y_WIDTH = 0;

    input [A_WIDTH-1:0] A;
    output [Y_WIDTH-1:0] Y;

    $__WOLF_SLICE_ #(
        .OFFSET(OFFSET),
        .A_WIDTH(A_WIDTH),
        .Y_WIDTH(Y_WIDTH)
    ) _TECHMAP_REPLACE_ (
        .A(A),
        .Y(Y)
    );

endmodule

