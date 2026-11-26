`timescale 1ns / 1ps

module tb_fmul32;

    reg clk = 0;
    reg  [31:0] a, b;
    wire [31:0] p;

    fmul32 uut (.clk(clk), .a(a), .b(b), .p(p));

    always #5 clk = ~clk;

    initial begin
        a = 32'h3FC00000; b = 32'h40000000; #20;   // 1.5 * 2
        $display("1.5 * 2     = %h (expected 40400000)", p);

        a = 32'hBF000000; b = 32'h3E800000; #20;   // -0.5 * 0.25
        $display("-0.5 * 0.25 = %h (expected BE000000)", p);

        a = 32'h40200000; b = 32'hC0800000; #20;   // 2.5 * -4
        $display("2.5 * -4    = %h (expected C1200000)", p);

        a = 32'h40400000; b = 32'h00000000; #20;   // 3 * 0
        $display("3 * 0       = %h (expected 00000000)", p);

        a = 32'h3DCCCCCD; b = 32'h3E4CCCCD; #20;   // 0.1 * 0.2
        $display("0.1 * 0.2   = %h (CPU gives 3CA3D70B, 1 bit less here because there is no rounding)", p);

        $finish;
    end

endmodule
