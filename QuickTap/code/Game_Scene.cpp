/*
 * Autor: Víctor
 * Fecha: 11/02/2018
 * Copyright (c) 2018 Víctor
 */

#include "Game_Scene.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>
#include <cstdlib>

using namespace basics;
using namespace std;

namespace QuickTap
{

    const Color Game_Scene::piece_colors[] =
    {
        {  95, 155, 255 },
        {  95, 255, 167 },
        { 255, 95,  118 },
        { 255, 212,  95 },
        { 125, 95,  255 },
        { 219, 95,  255 },
        { 103, 95,  255 },
        { 255, 197,  95 },
        { 189, 255,  95 },
        { 95, 255,  189 },
    };

    const unsigned Game_Scene::piece_color_count = sizeof(piece_colors) / 3;

    Game_Scene::Game_Scene()
    {
        canvas_width  = 1280;
        canvas_height = 720;

        stacked_pieces.reserve (100);
        falling_pieces.reserve (100);
    }

    bool Game_Scene::initialize ()
    {
        state             = LOADING;
		gameplay_state    = WAITING;
        current_direction = DIRECTION_X;
        suspended         = false;

        current_color_index = unsigned(rand ()) % piece_color_count;

        moving_piece =
        {
            Point3f  { 640 - 150, 0, 360 - 150 },
            Size3f   { 300, 0, 300 },
            Vector3f { 5, 0, 0 },
            next_color()
        };
        
		stacked_pieces.push_back (moving_piece);

        moving_piece.set_color (next_color());

        return true;
    }

    void Game_Scene::suspend ()
    {
        suspended = true;
    }

    void Game_Scene::resume ()
    {
        suspended = false;
    }

    void Game_Scene::handle (Event & event)
    {
        if (state == RUNNING)
        {
            switch (event.id)
            {
                case ID(touch-started):
                {
                    float touch_x = *event[ID(x)].as< var::Float > ();
                    float touch_y = *event[ID(y)].as< var::Float > ();
					
					if (gameplay_state == WAITING)
					{
						gameplay_state = PLAYING;
					}
					else
					if (gameplay_state == PLAYING)
					{
						if(touch_x < canvas_width  && touch_x > canvas_width - pause->get_width () + 90 && touch_y < canvas_height && touch_y > canvas_height - pause->get_height () + 90)
						{
							gameplay_state = PAUSED;
						}
						else
						{
							Piece::Cut_Result result = moving_piece.cut (stacked_pieces.back ());

//	                        if (!falling_piece.is_empty ())
//	                        {
//	                            falling_pieces.push_back ();
//	                        }

                            moving_piece.stop ();

                            stacked_pieces.push_back (moving_piece);

                            if(result.first == Piece::OUTSIDE)
                            {
                                game_over();
                            }

                            else
                            {
                                moving_piece.set_color (next_color ());

                                reset_moving_piece ();
                            }
						}
					}
                }
            }
        }
    }

    void Game_Scene::update (float time)
    {
        switch (state)
        {
            case LOADING:
                load ();
                break;

            case RUNNING:
                run  (time);
                break;

            case ERROR:
                break;
        }
    }

    void Game_Scene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended && state == RUNNING)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)
            {
                canvas->clear();

                for (auto & piece : stacked_pieces)
                {
                    piece.render(*canvas);
                }

                canvas->fill_rectangle
                (
                    { canvas_width - pause->get_width () / 2.f + 40, canvas_height - pause->get_height () / 2.f + 40 },
                    { pause->get_width (), pause->get_height () },
                      pause.get ()
                );

                moving_piece.render(*canvas);

				if (gameplay_state == PAUSED)
				{
                    canvas->fill_rectangle
                    (
                        { canvas_width / 2.f, canvas_height / 2.f },
                        { paused_continue->get_width (), paused_continue->get_height () },
                          paused_continue.get ()
                    );

                    canvas->fill_rectangle
                    (
                        { canvas_width / 2.f, canvas_height / 2.f },
                        { paused_leave->get_width (), paused_leave->get_height () },
                          paused_leave.get ()
                    );
				}

				else if (gameplay_state == GAME_OVER)
				{
                    canvas->fill_rectangle
                    (
                        { canvas_width / 2.f, canvas_height / 2.f },
                        { game_over_png->get_width (), game_over_png->get_height () },
                          game_over_png.get ()
                    );
				}
            }
        }
    }

    void Game_Scene::restart()
    {
        gameplay_state = WAITING;

        //  Vaciar stacked_pieces y falling_pieces.
        //  Reiniciar el estado de moving_piece
    }

    void Game_Scene::game_over()
    {
        gameplay_state = GAME_OVER;
    }

    void Game_Scene::reset_moving_piece()
    {
        switch (current_direction)
        {
            case DIRECTION_X:
                moving_piece.position = {stacked_pieces.back().position[0], 0, 0};
                moving_piece.speed = {0, 0, 5};
                current_direction = DIRECTION_Z;
                break;

            case DIRECTION_Z:
                moving_piece.position = {0, 0, stacked_pieces.back().position[2]};
                moving_piece.speed = {5, 0, 0};
                current_direction = DIRECTION_X;
                break;
        }
    }

    Color Game_Scene::next_color()
    {
        if (++current_color_index == piece_color_count)
        {
            current_color_index = 0;
        }

        return piece_colors[current_color_index];
    }

    void Game_Scene::load ()
    {
        if (!suspended)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                pause           =  Texture_2D::create (ID(test), context, "game_scene/pause.png");
                paused_continue =  Texture_2D::create (ID(test), context, "intro_scene/logo.png");
                paused_leave    =  Texture_2D::create (ID(test), context, "intro_scene/logo.png");
                game_over_png   =  Texture_2D::create (ID(test), context, "game_scene/GameOver.png");

                context->add (pause );            //Añade a este contexto la variable "pause"
                context->add (paused_continue);   //Añade a este contexto la variable "paused_continue"
                context->add (paused_leave   );   //Añade a este contexto la variable "paused_leave"
                context->add (game_over_png  );   //Añade a este contexto la variable "game_over_png"

                state = RUNNING;
            }
        }
    }

    void Game_Scene::run (float)
    {
		if (gameplay_state == PLAYING)
		{
			moving_piece.update();
		}
    }
}


