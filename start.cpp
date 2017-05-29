#include "C_App.hpp"
#include <gtkmm/application.h>

//////////////////////////////////////////////////////////////////////////////////
// [ on_command_line ]
//////////////////////////////////////////////////////////////////////////////////
namespace
{
int on_command_line(const Glib::RefPtr<Gio::ApplicationCommandLine>& command_line,
                    Glib::RefPtr<Gtk::Application>& app){
   /*
   int argc = 0;
   char** argv = command_line->get_arguments(argc);

   for(int i = 0; i < argc; ++i)
       std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
   */
   app->activate(); // Without activate() the window won't be shown.
   return EXIT_SUCCESS;
}
} // anonymous namespace
//////////////////////////////////////////////////////////////////////////////////
// [ main ]
//////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]){
  
   auto app = Gtk::Application::create(argc, 
				       argv, 
				       "org.gtkmm.example", 
				       Gio::APPLICATION_HANDLES_COMMAND_LINE | Gio::APPLICATION_NON_UNIQUE);

   app->signal_command_line().connect(sigc::bind(sigc::ptr_fun(&on_command_line), app), false);
   
   C_App CApp(argc, argv);

   //Shows the window and returns when it is closed.
   return(app->run(CApp));
}