module sig_splice_test(i0, i1, o0, o1);

    input wire [7:0] i0, i1;
    output wire [7:0] o0, o1;
    wire [7:0] a;
    wire [7:0] b;
    wire [7:0] c;

    blackbox bb0(
        .i({i0[3:0], i1[3:0]}),
        .o({a[3:0], b[3:0]})
    );

    assign a[7:4] = i0[7:4];
    assign b[7:4] = b[3:0];
    assign c[7:4] = i1[7:4];
    assign c[3:0] = 4'b1010;
    assign o0 = a;
    assign o1 = {b[3:0], c[3:0]};


endmodule

(* keep *)
module blackbox(
    input wire [7:0] i,
    output wire [7:0] o
);
    // This is a blackbox module, no implementation needed
endmodule