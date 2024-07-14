#include <gtk/gtk.h>
#include <pipewire/pipewire.h>
#include <stdio.h>

#include "main.h"
#include "pwnode.h"

static void
registry_event_global (void *data, uint32_t id, uint32_t permissions,
                       const char *type, uint32_t version,
                       const struct spa_dict *props)
{
  printf ("object: id:%u type:%s/%d\n", id, type, version);
}

static const struct pw_registry_events registry_events = {
  PW_VERSION_REGISTRY_EVENTS,
  .global = registry_event_global,
};

int
main (int argc, char *argv[])
{
  printf ("Compiled libpipewire: %s\n"
          "Linked libpipewire: %s\n",
          pw_get_headers_version (), pw_get_library_version ());

  // TODO; pipewire and glib are both taking argc and argv. This OK?
  pw_init (&argc, &argv);

  struct pn_context *pnctx;
  pnctx = pn_context_new ();

  struct spa_hook listener;
  spa_zero (listener);
  pw_registry_add_listener (pnctx->registry, &listener, &registry_events,
                            NULL);

  // # GTK
  GtkApplication *app;

  app = gtk_application_new ("com.github.stewi1014.pwnode", 0);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

  int status = g_application_run (G_APPLICATION (app), argc, argv);
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
