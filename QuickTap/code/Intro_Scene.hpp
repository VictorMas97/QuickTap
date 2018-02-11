/*
 * Autor: Víctor
 * Fecha: 11/02/2018
 * Copyright (c) 2018 Víctor
 */

#ifndef INTRO_SCENE_HEADER
#define INTRO_SCENE_HEADER

    #include <memory>
    #include <basics/Canvas>
    #include <basics/Scene>
    #include <basics/Texture_2D>
    #include <basics/Timer>

    namespace QuickTap
    {

        using basics::Timer;
        using basics::Canvas;
        using basics::Texture_2D;
        using basics::Graphics_Context;

        class Intro_Scene : public basics::Scene
        {
            ///Representa el estado de la escena en su conjunto.

            enum State
            {
                UNINITIALIZED,
                LOADING,
                FADING_IN,
                WAITING,
                FADING_OUT,
                FINISHED,
                ERROR
            };

        private:

            State    state;                                                              /// Estado de la escena.
            bool     suspended;                                                          /// true cuando la escena está en segundo plano y viceversa.

            unsigned canvas_width;                                                       /// Ancho de la resolución virtual usada para dibujar.
            unsigned canvas_height;                                                      /// Alto  de la resolución virtual usada para dibujar.

            Timer    timer;                                                              /// Cronómetro usado para medir intervalos de tiempo.

            float    opacity;                                                            /// Opacidad de la textura.

            std::shared_ptr < Texture_2D > logo_texture;                                 /// Textura que contiene la imagen del logo.

        public:

            Intro_Scene()                                                                /// Construye una Intro_Scene con sus variables.
            {
                state         = UNINITIALIZED;
                suspended     = true;
                canvas_width  = 1280;
                canvas_height =  720;
            }

            basics::Size2u get_view_size () override                                     /// Debuelve el tamaño de la pantalla.
            {
                return { canvas_width, canvas_height };
            }

            bool initialize () override;                                                 /// Inicializa las variables dependiendo del estado del juego.

            void suspend () override                                                     /// El juego pasa a segundo plano.
            {
                suspended = true;
            }
            void resume () override                                                      /// El juego pasa a primer plano.
            {
                suspended = false;
            }

            void update     (float time) override;                                       /// Llama a una función u otra dependiendo del estado del juego.

            void render     (basics::Graphics_Context::Accessor & context) override;     /// Se encarga de crear un canvas y de pintar las texturas.

        private:

            void update_loading    ();                                                   /// Carga el logo del juego.
            void update_fading_in  ();                                                   /// Aumenta la opacidad del logo a medida que pasa el tiempo
            void update_waiting    ();                                                   /// Espera unos segundos para iniciar la reducción de opacidad
            void update_fading_out ();                                                   /// Disminuye la opacidad del logo a medida que pasa el tiempo
        };
    }

#endif
