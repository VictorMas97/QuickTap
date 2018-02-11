/*
 * Autor: Víctor
 * Fecha: 11/02/2018
 * Copyright (c) 2018 Víctor
 */

#include <basics/Director>
#include <basics/enable>
#include <basics/Graphics_Resource_Cache>
#include <basics/Texture_2D>
#include <basics/Window>
#include <basics/opengles/Context>
#include <basics/opengles/Canvas_ES2>
#include <basics/opengles/OpenGL_ES2>

#include "Intro_Scene.hpp"

using namespace basics;
using namespace QuickTap;
using namespace std;

int main ()
{
    enable< basics::OpenGL_ES2 > ();                            // Es necesario habilitar un backend gráfico antes de nada

    director.run_scene (shared_ptr< Scene >(new Intro_Scene));  // Se crea una escena y se inicia mediante el Director:

    return 0;
}

void keep_links ()  // El linker tiende a eliminar código que cree que no se usa y, cuando se termina usando, lo echa en falta provocando errores.
{
    const bool &c = Window::can_be_instantiated;

    (void)Window::can_be_instantiated;
    Window::Accessor window;
    Window::create_window (0);
    Window::get_window (0);
    Graphics_Resource_Cache cache;
    opengles::Context::create(window, &cache);
    Canvas::Factory f = opengles::Canvas_ES2::create;
    Texture_2D::register_factory (0, 0);
}
