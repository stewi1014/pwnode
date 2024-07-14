#pragma once

#include <glib.h>
#include <pipewire/pipewire.h>

struct pn_context
{
  GSource gsource;
  struct pw_main_loop *main_loop;
  struct pw_context *context;
  struct pw_core *core;
  struct pw_registry *registry;
};

struct pn_context *pn_context_new ();
void pn_context_free (struct pn_context *);
