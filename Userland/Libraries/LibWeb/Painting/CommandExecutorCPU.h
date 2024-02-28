/*
 * Copyright (c) 2023, Aliaksandr Kalenik <kalenik.aliaksandr@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/MaybeOwned.h>
#include <LibWeb/Painting/RecordingPainter.h>

namespace Web::Painting {

class CommandExecutorCPU : public CommandExecutor {
public:
    CommandResult draw_glyph_run(Vector<Gfx::DrawGlyphOrEmoji> const& glyph_run, Color const&) override;
    CommandResult draw_glyph_run(Gfx::Painter& painter, Vector<Gfx::DrawGlyphOrEmoji> const& glyph_run, Color const&);
    CommandResult draw_text(Gfx::IntRect const& rect, String const& raw_text, Gfx::TextAlignment alignment, Color const&, Gfx::TextElision, Gfx::TextWrapping, Optional<NonnullRefPtr<Gfx::Font>> const&) override;
    CommandResult fill_rect(Gfx::IntRect const& rect, Color const&) override;
    CommandResult draw_scaled_bitmap(Gfx::IntRect const& dst_rect, Gfx::Bitmap const& bitmap, Gfx::IntRect const& src_rect, Gfx::Painter::ScalingMode scaling_mode) override;
    CommandResult draw_scaled_immutable_bitmap(Gfx::IntRect const& dst_rect, Gfx::ImmutableBitmap const&, Gfx::IntRect const& src_rect, Gfx::Painter::ScalingMode scaling_mode) override;
    CommandResult set_clip_rect(Gfx::IntRect const& rect) override;
    CommandResult clear_clip_rect() override;
    CommandResult push_stacking_context(float opacity, bool is_fixed_position, Gfx::IntRect const& source_paintable_rect, Gfx::IntPoint post_transform_translation, CSS::ImageRendering image_rendering, StackingContextTransform transform, Optional<StackingContextMask> mask) override;
    CommandResult pop_stacking_context() override;
    CommandResult paint_linear_gradient(Gfx::IntRect const&, Web::Painting::LinearGradientData const&) override;
    CommandResult paint_outer_box_shadow(PaintOuterBoxShadowParams const&) override;
    CommandResult paint_inner_box_shadow(PaintOuterBoxShadowParams const&) override;
    CommandResult paint_text_shadow(int blur_radius, Gfx::IntRect const& shadow_bounding_rect, Gfx::IntRect const& text_rect, Span<Gfx::DrawGlyphOrEmoji const>, Color const&, int fragment_baseline, Gfx::IntPoint const& draw_location) override;
    CommandResult fill_rect_with_rounded_corners(Gfx::IntRect const&, Color const&, Gfx::AntiAliasingPainter::CornerRadius const& top_left_radius, Gfx::AntiAliasingPainter::CornerRadius const& top_right_radius, Gfx::AntiAliasingPainter::CornerRadius const& bottom_left_radius, Gfx::AntiAliasingPainter::CornerRadius const& bottom_right_radius) override;
    CommandResult fill_path_using_color(Gfx::Path const&, Color const&, Gfx::Painter::WindingRule winding_rule, Gfx::FloatPoint const& aa_translation) override;
    CommandResult fill_path_using_paint_style(Gfx::Path const&, Gfx::PaintStyle const& paint_style, Gfx::Painter::WindingRule winding_rule, float opacity, Gfx::FloatPoint const& aa_translation) override;
    CommandResult stroke_path_using_color(Gfx::Path const&, Color const& color, float thickness, Gfx::FloatPoint const& aa_translation) override;
    CommandResult stroke_path_using_paint_style(Gfx::Path const& path, Gfx::PaintStyle const& paint_style, float thickness, float opacity, Gfx::FloatPoint const& aa_translation) override;
    CommandResult draw_ellipse(Gfx::IntRect const& rect, Color const& color, int thickness) override;
    CommandResult fill_ellipse(Gfx::IntRect const& rect, Color const& color, Gfx::AntiAliasingPainter::BlendMode blend_mode) override;
    CommandResult draw_line(Color const&, Gfx::IntPoint const& from, Gfx::IntPoint const& to, int thickness, Gfx::Painter::LineStyle style, Color const& alternate_color) override;
    CommandResult draw_signed_distance_field(Gfx::IntRect const& rect, Color const&, Gfx::GrayscaleBitmap const& sdf, float smoothing) override;
    CommandResult paint_frame(Gfx::IntRect const& rect, Palette const&, Gfx::FrameStyle) override;
    CommandResult apply_backdrop_filter(Gfx::IntRect const& backdrop_region, Web::CSS::ResolvedBackdropFilter const& backdrop_filter) override;
    CommandResult draw_rect(Gfx::IntRect const& rect, Color const&, bool rough) override;
    CommandResult paint_radial_gradient(Gfx::IntRect const& rect, Web::Painting::RadialGradientData const& radial_gradient_data, Gfx::IntPoint const& center, Gfx::IntSize const& size) override;
    CommandResult paint_conic_gradient(Gfx::IntRect const& rect, Web::Painting::ConicGradientData const& conic_gradient_data, Gfx::IntPoint const& position) override;
    CommandResult draw_triangle_wave(Gfx::IntPoint const& p1, Gfx::IntPoint const& p2, Color const&, int amplitude, int thickness) override;
    CommandResult sample_under_corners(u32 id, CornerRadii const&, Gfx::IntRect const&, CornerClip) override;
    CommandResult blit_corner_clipping(u32 id) override;
    CommandResult paint_borders(DevicePixelRect const& border_rect, CornerRadii const& corner_radii, BordersDataDevicePixels const& borders_data) override;
    CommandResult create_foreground_text_alpha_mask(u32 id, Gfx::IntRect const& background_rect) override;
    CommandResult blit_foreground_text_alpha_mask(u32 id) override;
    CommandResult push_alpha_mask_id(u32 id) override;
    CommandResult pop_alpha_mask_id() override;

    bool would_be_fully_clipped_by_painter(Gfx::IntRect) const override;

    bool needs_prepare_glyphs_texture() const override { return false; }
    void prepare_glyph_texture(HashMap<Gfx::Font const*, HashTable<u32>> const&) override {};

    bool needs_update_immutable_bitmap_texture_cache() const override { return false; }
    void update_immutable_bitmap_texture_cache(HashMap<u32, Gfx::ImmutableBitmap const*>&) override {};

    CommandExecutorCPU(Gfx::Bitmap& bitmap);

private:
    Gfx::Bitmap& m_target_bitmap;
    Vector<RefPtr<BorderRadiusCornerClipper>> m_corner_clippers;

    struct StackingContext {
        MaybeOwned<Gfx::Painter> painter;
        float opacity;
        Gfx::IntRect destination;
        Gfx::Painter::ScalingMode scaling_mode;
        Optional<StackingContextMask> mask = {};
    };

    struct ForegroundTextAlphaMask {
        RefPtr<Gfx::Bitmap> background_canvas;
        RefPtr<Gfx::Bitmap> mask_canvas;
        OwnPtr<Gfx::Painter> background_painter;
        OwnPtr<Gfx::Painter> mask_painter;
        Gfx::IntRect destination_rect;
    };

    [[nodiscard]] Gfx::Painter const& painter() const {
        if (m_alpha_mask_stack.size() > 0) {
            // If rendering to an alpha mask, by default render to the background canvas.
            return *(m_foreground_text_alpha_masks[m_alpha_mask_stack.last()]->background_painter);
        }
        return *stacking_contexts.last().painter;
    }
    [[nodiscard]] Gfx::Painter& painter() {
        if (m_alpha_mask_stack.size() > 0) {
            // If rendering to an alpha mask, by default render to the background canvas.
            return *(m_foreground_text_alpha_masks[m_alpha_mask_stack.last()]->background_painter);
        }
        return *stacking_contexts.last().painter;
    }
    [[nodiscard]] Gfx::Painter const& target_painter() const { return *stacking_contexts.last().painter; }
    [[nodiscard]] Gfx::Painter& target_painter() { return *stacking_contexts.last().painter; }

    Vector<StackingContext> stacking_contexts;
    Vector<OwnPtr<ForegroundTextAlphaMask>> m_foreground_text_alpha_masks;
    Vector<u32> m_alpha_mask_stack;
};

}
