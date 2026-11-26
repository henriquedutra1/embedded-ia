// Float32 multiplier (IEEE-754 single precision)
// Simplified: no rounding (truncation), no NaN/infinity, denormals = 0
// Result is ready 1 clock cycle after a and b change

module fmul32 (
    input         clk,
    input  [31:0] a,
    input  [31:0] b,
    output [31:0] p
);

    reg [47:0] m;   // mantissa product
    reg [9:0]  e;   // sum of exponents
    reg        s;   // sign
    reg        z;   // one input is zero

    always @(posedge clk) begin
        m <= {1'b1, a[22:0]} * {1'b1, b[22:0]};
        e <= a[30:23] + b[30:23];
        s <= a[31] ^ b[31];
        z <= (a[30:23] == 0) || (b[30:23] == 0);
    end

    // normalization: if the product is >= 2, shift by one and add 1 to the exponent
    wire [9:0]  e2 = m[47] ? e - 126 : e - 127;
    wire [22:0] f  = m[47] ? m[46:24] : m[45:23];

    assign p = (z || e2[9] || e2 == 0) ? 32'd0 : {s, e2[7:0], f};

endmodule
