/*
 * Autor: Víctor
 * Fecha: 11/02/2018
 * Copyright (c) 2018 Víctor
 */

#include "Intro_Scene.hpp"
#include "Menu_Scene.hpp"
#include <basics/Canvas>
#include <basics/Director>

using namespace basics;
using namespace std;

namespace QuickTap
{

    bool Intro_Scene::initialize ()
    {
        if (state == UNINITIALIZED)
        {
            state = LOADING;
        }
        else
        {
            timer.reset ();

            opacity = 0.f;
            state   = FADING_IN;
        }
        return true;
    }

    void Intro_Scene::update (float time)
    {
        if (!suspended) switch (state)
        {
            case LOADING:
                update_loading    ();
                break;

            case FADING_IN:
                update_fading_in  ();
                break;

            case WAITING:
                update_waiting    ();
                break;

            case FADING_OUT:
                update_fading_out ();
                break;

            default:
                break;
        }
    }

    void Intro_Scene::render (Graphics_Context::Accessor & context)
    {
        if (!suspended)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));  // El canvas se puede haber creado previamente, en cuyo caso solo hay que pedirlo

            if (!canvas) // Si no se ha creado previamente, hay que crearlo una vez
            {
                 canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)  // Si el canvas se ha podido obtener o crear, se puede dibujar con él
            {
                canvas->clear ();

                if (logo_texture)
                {
                    canvas->set_opacity (opacity);

                    canvas->fill_rectangle
                    (
                        { canvas_width * .5f, canvas_height * .5f },
                        { logo_texture->get_width (), logo_texture->get_height () },
                          logo_texture. get ()
                    );
                }
            }
        }
    }

    void Intro_Scene::update_loading ()
    {
        Graphics_Context::Accessor context = director.lock_graphics_context ();

        if (context)
        {
            logo_texture = Texture_2D::create (0, context, "intro_scene/logo.png");    // Se carga la textura del logo

            if (logo_texture)                                                          // Se comprueba si la textura se ha podido cargar correctamente:
            {
                context->add (logo_texture);

                timer.reset ();

                opacity = 0.f;
                state   = FADING_IN;
            }
            else
                state   = ERROR;
        }
    }

    void Intro_Scene::update_fading_in ()
    {
        float elapsed_seconds = timer.get_elapsed_seconds ();

        if (elapsed_seconds < 1.f)
        {
            opacity = elapsed_seconds;      // Se aumenta la opacidad del logo a medida que pasa el tiempo
        }
        else
        {
            timer.reset ();

            opacity = 1.f;
            state   = WAITING;
        }
    }

    void Intro_Scene::update_waiting ()
    {
        if (timer.get_elapsed_seconds () > 2.f)    // Se esperan dos segundos sin hacer nada:
        {
            timer.reset ();

            state = FADING_OUT;
        }
    }

    void Intro_Scene::update_fading_out ()
    {
        float elapsed_seconds = timer.get_elapsed_seconds ();

        if (elapsed_seconds < .5f)
        {
            opacity = 1.f - elapsed_seconds * 2.f;                    // Se reduce la opacidad de 1 a 0 en medio segundo
        }
        else
        {
            state = FINISHED;

            director.run_scene (shared_ptr< Scene >(new Menu_Scene)); // Cuando el faceout se ha completado, se lanza la siguiente escena:
        }
    }
}
