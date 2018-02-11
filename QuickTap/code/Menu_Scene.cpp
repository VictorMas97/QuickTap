/*
 * Autor: Víctor
 * Fecha: 11/02/2018
 * Copyright (c) 2018 Víctor
 */

#include "Menu_Scene.hpp"
#include "Game_Scene.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Transformation>

using namespace basics;
using namespace std;

namespace QuickTap
{

    Menu_Scene::Menu_Scene()
    {
        state         = LOADING;
        suspended     = true;
        canvas_width  = 1280;
        canvas_height =  720;
    }

    bool Menu_Scene::initialize ()
    {
        for (auto & option : options)
        {
            option.is_pressed = false;
        }

        return true;
    }

    void Menu_Scene::handle (basics::Event & event)
    {
        if (state == READY)                     // Se descartan los eventos cuando la escena está LOADING
        {
            switch (event.id)
            {
                case ID(touch-moved):
                {

                    Point2f touch_location = { *event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };  // Se determina qué opción se ha tocado

                    int     option_touched = option_at (touch_location);

                    for (int index = 0; index < number_of_options; ++index)  // Solo se puede tocar una opción a la vez (para evitar selecciones múltiples), solo una se considera presionada
                    {
                        options[index].is_pressed = index == option_touched;
                    }

                    break;
                }

                case ID(touch-ended):           // El usuario deja de tocar la pantalla
                {
                    // Se "sueltan" todas las opciones:

                    for (auto & option : options) option.is_pressed = false;

                    // Se determina qué opción se ha dejado de tocar la última y se actúa como corresponda:

                    Point2f touch_location = { *event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };

                    if (option_at (touch_location) == PLAY)
                    {
                        director.run_scene (shared_ptr< Scene >(new Game_Scene));
                    }

                    break;
                }
            }
        }
    }

    void Menu_Scene::update (float time)
    {
        if (!suspended) if (state == LOADING)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                atlas.reset (new Atlas("menu_scene/main-menu.sprites", context));           // Se carga el atlas

                state = atlas->good () ? READY : ERROR;                                     // Si el atlas se ha podido cargar el estado es READY y, en otro caso, es ERROR

                if (state == READY)                                                         // Si el atlas está disponible, se inicializan los datos de las opciones del menú:
                {
                    configure_options ();
                }
            }
        }
    }

    void Menu_Scene::render (Graphics_Context::Accessor & context)
    {
        if (!suspended)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas)); // El canvas se puede haber creado previamente, en cuyo caso solo hay que pedirlo

            if (!canvas)                                                    // Si no se ha creado previamente, hay que crearlo una vez
            {
                 canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)                           // Si el canvas se ha podido obtener o crear, se puede dibujar con él
            {
                canvas->clear ();

                if (state == READY)
                {
                    for (auto & option : options) // Se dibuja el slice de cada una de las opciones del menú
                    {
                        canvas->set_transform
                        (
                            scale_then_translate_2d
                            (
                                  option.is_pressed ? 0.75f : 1.f,              // Escala de la opción
                                { option.position[0], option.position[1] }      // Traslación
                            )
                        );

                        canvas->fill_rectangle ({ 0.f, 0.f }, { option.slice->width, option.slice->height }, option.slice, CENTER | TOP);
                    }

                    canvas->set_transform (Transformation2f());   // Se restablece la transformación aplicada a las opciones para que no afecte a dibujos posteriores realizados con el mismo canvas
                }
            }
        }
    }

    void Menu_Scene::configure_options ()
    {
        // Se asigna un slice del atlas a cada opción del menú según su ID:

        options[PLAY   ].slice = atlas->get_slice (ID(play)   );
        options[SCORES ].slice = atlas->get_slice (ID(scores) );
        options[HELP   ].slice = atlas->get_slice (ID(help)   );
        options[CREDITS].slice = atlas->get_slice (ID(credits));

        float menu_height = 0;

        for (auto & option : options) menu_height += option.slice->height;   // Se calcula la altura total del menú

        float option_top = canvas_height / 2.f + menu_height / 2.f;          // Se calcula la posición del borde superior del menú en su conjunto de modo que quede centrado verticalmente

        for (unsigned index = 0; index < number_of_options; ++index)         // Se establece la posición del borde superior de cada opción
        {
            options[index].position = Point2f{ canvas_width / 2.f, option_top };

            option_top -= options[index].slice->height;
        }

        initialize ();        // Se restablece la presión de cada opción
    }

    int Menu_Scene::option_at (const Point2f & point)
    {
        for (int index = 0; index < number_of_options; ++index)
        {
            const Option & option = options[index];

            if
            (
                point[0] > option.position[0] - option.slice->width  &&
                point[0] < option.position[0] + option.slice->width  &&
                point[1] > option.position[1] - option.slice->height &&
                point[1] < option.position[1] + option.slice->height
            )
            {
                return index;
            }
        }

        return -1;
    }
}
