/*
 * Autor: Víctor
 * Fecha: 11/02/2018
 * Copyright (c) 2018 Víctor
 */

#ifndef MENU_SCENE_HEADER
#define MENU_SCENE_HEADER

    #include <memory>
    #include <basics/Atlas>
    #include <basics/Canvas>
    #include <basics/Point>
    #include <basics/Scene>
    #include <basics/Size>
    #include <basics/Timer>

    namespace QuickTap
    {

        using basics::Atlas;
        using basics::Timer;
        using basics::Canvas;
        using basics::Point2f;
        using basics::Size2f;
        using basics::Texture_2D;
        using basics::Graphics_Context;

        class Menu_Scene : public basics::Scene
        {
            ///Representa el estado de la escena en su conjunto.

            enum State
            {
                LOADING,
                READY,
                ERROR
            };

            enum Option_Id
            {
                PLAY,
                SCORES,
                HELP,
                CREDITS
            };

            struct Option
            {
                const Atlas::Slice * slice;
                Point2f position;
                float   is_pressed;
            };

            static const unsigned number_of_options = 4;

        private:

            State    state;                                                              /// Estado de la escena.
            bool     suspended;                                                          /// true cuando la escena está en segundo plano y viceversa.

            unsigned canvas_width;                                                       /// Ancho de la resolución virtual usada para dibujar.
            unsigned canvas_height;                                                      /// Alto  de la resolución virtual usada para dibujar.

            Option   options[number_of_options];                                         /// Datos de las opciones del menú

            std::unique_ptr< Atlas > atlas;                                              /// Atlas que contiene las imágenes de las opciones del menú

        public:

            Menu_Scene();                                                                /// Construye una Menu_Scene vacia.

            basics::Size2u get_view_size () override                                     /// Debuelve el tamaño de la pantalla.
            {
                return { canvas_width, canvas_height };
            }

            bool initialize () override;                                                 /// Inicializa las variables, pone que ningun botone este presionado.

            void suspend () override                                                     /// El juego pasa a segundo plano.
            {
                suspended = true;
            }

            void resume () override                                                      /// El juego pasa a primer plano.
            {
                suspended = false;
            }

            void handle     (basics::Event & event) override;                            /// Se llama a este método cuando el usuario toca la pantalla, es decir, un input.

            void update     (float time) override;                                       /// Carga el atlas y llama a una función u otra dependiendo del estado del juego.

            void render     (basics::Graphics_Context::Accessor & context) override;     /// Se encarga de crear un canvas y de pintar las texturas y reescalarlas cuando toque.

        private:

            void configure_options ();                                                   /// Establece las propiedades de cada opción si se ha podido cargar el atlas.

            int option_at (const Point2f & point);                                       /// Devuelve el índice de la opción que se encuentra bajo el punto indicado.
        };
    }

#endif
