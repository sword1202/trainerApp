#ifndef NANOVG_BASE_HEADER
#define NANOVG_BASE_HEADER

// Create flags

enum NVGcreateFlags {
    // Flag indicating if geometry based anti-aliasing is used (may not be
    // needed when using MSAA).
            NVG_ANTIALIAS = 1 << 0,
    // Flag indicating if strokes should be drawn using stencil buffer.
    // The rendering will be a little slower, but path overlaps
    // (i.e. self-intersecting or sharp turns) will be drawn just once.
            NVG_STENCIL_STROKES = 1 << 1,
    // Flag indicating if double buffering scheme is used.
            NVG_DOUBLE_BUFFER = 1 << 12,
    // Flag indicating if triple buffering scheme is used.
            NVG_TRIPLE_BUFFER = 1 << 13,
    // Flag indicating that additional debug checks are done.
            NVG_DEBUG = 1 << 2,
};

#endif