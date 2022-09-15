.global transform_png_asm_red
.global transform_png_asm_green
.global transform_png_asm_blue
.type	transform_png_asm_red, %function
.type	transform_png_asm_green, %function
.type	transform_png_asm_blue, %function

// void transform_png(unsigned char *in, unsigned char *out, unsigned int h, unsigned int w, int color) {
//         for (int i = 0; i < h; i++) {
//                 for (int j = 0; j < w; j++) {
//                         tr[w * i + j] = orig[(w * i + j) * 3 + color];
//                 }
//         }
// }

transform_png_asm_red:
    // in - x0
    // out - x1
    // h - x2
    // w - x3
    // out + h * w - x5

    mul x4, x2, x3
    add x5, x1, x4

    transform_png_asm_red_loop:
        cmp x1, x4
        b.HS transform_png_asm_red_loop_end

        ldrb w7, [x0, #0]
        strb w7, [x1]


        add x0, x0, #4
        add x1, x1, #1
        b transform_png_asm_red_loop
        transform_png_asm_red_loop_end:
    ret

transform_png_asm_green:
    // in - x0
    // out - x1
    // h - x2
    // w - x3
    // out + h * w - x5

    mul x4, x2, x3
    add x5, x1, x4

    transform_png_asm_green_loop:
        cmp x1, x4
        b.HS transform_png_asm_green_loop_end

        ldrb w7, [x0, #1]
        strb w7, [x1]


        add x0, x0, #4
        add x1, x1, #1
        b transform_png_asm_green_loop
        transform_png_asm_green_loop_end:
    ret

transform_png_asm_blue:
    // in - x0
    // out - x1
    // h - x2
    // w - x3
    // out + h * w - x5

    mul x4, x2, x3
    add x5, x1, x4

    transform_png_asm_blue_loop:
        cmp x1, x4
        b.HS transform_png_asm_blue_loop_end

        ldrb w7, [x0, #2]
        strb w7, [x1]


        add x0, x0, #4
        add x1, x1, #1
        b transform_png_asm_blue_loop
        transform_png_asm_blue_loop_end:
    ret
