/*!
 * \file life.h
 *
 * \brief An implementation of the Game of Life
 *
 * The Life universe is represented by an array of cell groups, each of which
 * is a co-ordinate followed by a bitmap of the cells at that position. The
 * groups are organized as a sequence of rows. Each row starts with a dummy
 * group containing the Y position of the following groups, each of which has
 * only an X position. X and Y (real and dummy) are distinguished from each
 * other by their sign. The array is terminated by a dummy Y value of -1.
 * X co-ordinates differ by the word size between adjacent cell groups.
 *
 * Y values have the same sign as the terminator, so they are negative and
 * X values are positive. Y values are all less than the terminator, so
 * they increase in order to make the terminator sort last. X values are
 * all greater than Y values, so they decrease in order to make the Y value
 * following the line sort after the line.
 *
 * $Copyright: (C) 2003 Tony Finch <dot@dotat.at> $
 *
 * $dotat: life/life.h,v 1.16 2003/12/17 10:02:19 fanf2 Exp $
 */
#ifndef LIFE_H
#define LIFE_H

#include <limits.h>
#include <stddef.h>

/*!
 * \brief The type of cell co-ordinates.
 * A signed integer type.
 */
typedef int life_pos;

/*!
 * \brief The type of cell bitmaps.
 * An unsigned integer type.
 */
typedef unsigned life_bmp;

/*!
 * \brief The type of a cell group.
 * A pair of a position and a bitmap.
 */
typedef struct life_cells {
	life_pos pos;
	life_bmp bmp;
} life_cells;

/*!
 * \brief How many bits to calculate at once.
 * We need space in a word for two bitmaps plus an extra bit.
 */
#define WORDSIZE ((int)sizeof(life_bmp) * CHAR_BIT)

/*!\brief X co-ordinate of the centre of the universe. */
#define CENTRE_X (INT_MAX / 2 + 1)

/*!\brief Y co-ordinate of the centre of the universe. */
#define CENTRE_Y (INT_MIN / 2)

/**********************************************************************/

/*!
 * \brief Storage for the Game of Life context.
 */
typedef struct life_ctx {
	life_cells *cells, *end, *new;
	unsigned long workdone;
	unsigned long generation;
	life_cells *space, *old;
	size_t size;
	life_pos minx, miny, maxx, maxy;
	void *aux;
} life_ctx;

/*!
 * \brief Initialize a Life context.
 * \param ctx The Game of Life context.
 * \param aux User's auxiliary data.
 * \param minx Inner minimum X co-ordinate of the display.
 * \param miny Inner minimum Y co-ordinate of the display.
 * \param maxx Outer maximum X co-ordinate of the display.
 * \param maxy Outer maximum Y co-ordinate of the display.
 */
void life_init(life_ctx *ctx, void *aux,
    life_pos minx, life_pos miny, life_pos maxx, life_pos maxy);

/*!
 * \brief Increase the space available for new cells.
 * \param ctx The Game of Life context.
 * \param space The minimum number of life_cells to allow for.
 */
void life_alloc(life_ctx *ctx, size_t space);

/*!
 * \brief Print out some debug info from the Life context.
 * \param ctx The Game of Life context.
 */
void life_debug(life_ctx *ctx);

/*!
 * \brief Find the outer Y maximum of the bounding box.
 * \param ctx The Game of Life context.
 * \returns The outer Y maximum of the bounding box.
 */
life_pos life_maxy(life_ctx *ctx);

/**********************************************************************/

/*!
 * \brief Redraw the Life universe.
 * \param ctx The Game of Life context.
 */
void life_draw(life_ctx *ctx);

/*!
 * \brief The type of functions that update the display of a cell group.
 * \param ctx The Game of Life context.
 * \param x Absolute X co-ordinate of the group.
 * \param y Absolute Y co-ordinate of the group.
 * \param old The old state of the cell group.
 * \param new The new state of the cell group.
 * Performs clipping and translation of the co-ordinates into the
 * rectangle ctx->minx, ctx->miny, ctx->maxx, ctx->maxy.
 */
typedef void life_draw_bmp_fn(life_ctx *ctx,
    life_pos x, life_pos y, life_bmp old, life_bmp new);

/*!
 * \brief Example implementation of a life_draw_bmp_fn().
 */
life_draw_bmp_fn life_draw_bmp_cellwise;

/*!
 * \brief The user's bitmap display function.
 */
life_draw_bmp_fn life_draw_bmp;

/*!
 * \brief The type of functions that display a cell.
 * \param ctx The Game of Life context.
 * \param x Relative X co-ordinate of the cell.
 * \param y Relative Y co-ordinate of the cell.
 * The co-ordinates are relative to draw->minx, draw->miny.
 */
typedef void life_draw_cell_fn(life_ctx *ctx, life_pos x, life_pos y);

/*!
 * \brief User-defined functions called by life_draw_bmp_cellwise().
 */
life_draw_cell_fn life_draw_dead, life_draw_live;

/*!
 * \brief The type of functions called at various points.
 * \param ctx The Game of Life context.
 */
typedef void life_hook_fn(life_ctx *ctx);

/*!
 * \brief Clear the display.
 * Called at the start of life_draw().
 */
life_hook_fn life_draw_clear;

/*!
 * \brief Update the display.
 * Called at the end of life_draw() and life_step().
 */
life_hook_fn life_draw_flush;

/**********************************************************************/

/*!
 * \brief Compute and display the next generation of Life.
 * \param ctx The Game of Life context.
 * Does memory and display management for the inner life() routine.
 */
void life_step(life_ctx *ctx);

/*!
 * \brief Type of functions that create a life form.
 * \param new Where to put the life form.
 * \param cx X co-ordinate of the centre of the life form.
 * \param cy Y co-ordinate of the centre of the life form.
 * \param n Size or other life form parameter.
 * \return Pointer to free space after new.
 */
typedef life_cells *life_form(life_cells *new,
    life_pos cx, life_pos cy, int n);

life_form life_form_bunnies;
life_form life_form_glider;
life_form life_form_linepuffer;
life_form life_form_puffer;
life_form life_form_rpent;

/**********************************************************************/

/*!
 * \brief Compute the next generation of the Game of Life.
 * \param this The current state of the known universe.
 * \param new Where to put the replacement universe.
 * \param ctx Context pointer to pass to life_draw_bmp().
 * \return Pointer to free space after new.
 * \note \c new must have three times the space used by \c this.
 * This is the low-level routine called by life_step().
 * It calls life_draw_bmp() to update the display as it proceeds.
 */
life_cells *life(life_cells *this, life_cells *new, void *ctx);

/*!
 * \brief A parallel half-adder.
 * \param s0 Bit 0 of the sum
 * \param s1 Bit 1 of the sum
 * \param a0 First argument
 * \param a1 Second argument
 * \note s1 must not be the same as a1 or a2
 */
#define ADD2(s0,s1,a0,a1) do {		\
	    s1 = (a0) & (a1);		\
	    s0 = (a0) ^ (a1);		\
	} while(0)

/*!
 * \brief A parallel full-adder.
 * \param s0 Bit 0 of the sum
 * \param s1 Bit 1 of the sum
 * \param a0 First argument
 * \param a1 Second argument
 * \param a2 Third argument
 * \note s0 must not be the same as a2
 */
#define ADD3(s0,s1,a0,a1,a2) do {	\
	    life_bmp c0, c1;		\
	    ADD2(s0,c0,a0,a1);		\
	    ADD2(s0,c1,s0,a2);		\
	    s1 = c0 | c1;		\
	} while(0)

#endif /* LIFE_H */
