#include <gtk/gtk.h>
#include <pipewire/pipewire.h>
#include <stdio.h>

#include "pwnode.h"

int
main (int argc, char *argv[])
{
  GtkApplication *app;
  int status;

  printf ("Compiled libpipewire: %s\n"
          "Linked libpipewire: %s\n",
          pw_get_headers_version (), pw_get_library_version ());

  app = gtk_application_new ("com.github.stewi1014.pwnode", 0);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

static void
activate (GtkApplication *app, gpointer user_data)
{
  GtkBuilder *builder;
  GtkWindow *window;

  builder = gtk_builder_new_from_resource (
      "/com/github/stewi1014/pwnode/window.ui");
  window = GTK_WINDOW (gtk_builder_get_object (builder, "window"));

  gtk_window_set_application (window, app);
  gtk_widget_set_visible (GTK_WIDGET (window), TRUE);

  g_object_unref (builder);
}
