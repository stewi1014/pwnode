#include "pwnode.h"

#include <glib.h>
#include <pipewire/pipewire.h>
#include <spa/utils/result.h>

static gboolean
pipewire_source_prepare (GSource *source, gint *timeout_)
{
  *timeout_ = -1;
  return FALSE;
}

static gboolean
pipewire_source_check (GSource *source)
{
  return FALSE; // TODO; understand this function
}

static gboolean
pipewire_source_dispatch (GSource *source, GSourceFunc callback,
                          gpointer user_data)
{
  struct pn_context *pnctx = (struct pn_context *)source;
  int result;

  result = pw_loop_iterate (pw_main_loop_get_loop (pnctx->main_loop), 0);
  if (result < 0)
    g_warning ("pw_loop_iterate failed: %s", spa_strerror (result));

  return TRUE;
}

static void
pipewire_source_finalize (GSource *source)
{
  struct pn_context *pnctx = (struct pn_context *)source;

  pw_registry_destroy (pnctx->registry, 0);
  pw_core_disconnect (pnctx->core);
  pw_context_destroy (pnctx->context);

  pw_loop_leave (pw_main_loop_get_loop (pnctx->main_loop));
  pw_main_loop_destroy (pnctx->main_loop);
}

static GSourceFuncs pipewire_source_funcs
    = { pipewire_source_prepare, pipewire_source_check,
        pipewire_source_dispatch, pipewire_source_finalize };

struct pn_context *
pn_context_new ()
{
  struct pn_context *pnctx;
  pnctx = (struct pn_context *)g_source_new (&pipewire_source_funcs,
                                             sizeof (struct pn_context));

  pnctx->main_loop = pw_main_loop_new (NULL);
  pnctx->context
      = pw_context_new (pw_main_loop_get_loop (pnctx->main_loop), NULL, 0);
  pnctx->core = pw_context_connect (pnctx->context, NULL, 0);
  pnctx->registry = pw_core_get_registry (pnctx->core, PW_VERSION_REGISTRY, 0);

  g_source_set_name (&pnctx->gsource, "[pwnode] PipeWire");

  g_source_add_unix_fd (
      &pnctx->gsource,
      pw_loop_get_fd (pw_main_loop_get_loop (pnctx->main_loop)),
      G_IO_IN | G_IO_ERR);

  pw_loop_enter (pw_main_loop_get_loop (pnctx->main_loop));
  g_source_attach (&pnctx->gsource, NULL);
  g_source_unref (&pnctx->gsource);

  return pnctx;
}

void
pn_context_free (struct pn_context *pnctx)
{
  free (pnctx);
}
