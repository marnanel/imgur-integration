#ifndef EOG_IMGUR_PLUGIN_H
#define EOG_IMGUR_PLUGIN_H

#include <glib.h>
#include <glib-object.h>
#include <eog/eog-plugin.h>

G_BEGIN_DECLS

#define EOG_TYPE_IMGUR_PLUGIN		(eog_imgur_plugin_get_type ())
#define EOG_IMGUR_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), EOG_TYPE_IMGUR_PLUGIN, EogImgurPlugin))
#define EOG_IMGUR_PLUGIN_CLASS(k)		G_TYPE_CHECK_CLASS_CAST((k),      EOG_TYPE_IMGUR_PLUGIN, EogImgurPluginClass)
#define EOG_IS_IMGUR_PLUGIN(o)	        (G_TYPE_CHECK_INSTANCE_TYPE ((o), EOG_TYPE_IMGUR_PLUGIN))
#define EOG_IS_IMGUR_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k),    EOG_TYPE_IMGUR_PLUGIN))
#define EOG_IMGUR_PLUGIN_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o),  EOG_TYPE_IMGUR_PLUGIN, EogImgurPluginClass))

/* Private structure type */
typedef struct _EogImgurPluginPrivate	EogImgurPluginPrivate;

/*
 * Main object structure
 */
typedef struct _EogImgurPlugin		EogImgurPlugin;

struct _EogImgurPlugin
{
	EogPlugin parent_instance;
};

/*
 * Class definition
 */
typedef struct _EogImgurPluginClass	EogImgurPluginClass;

struct _EogImgurPluginClass
{
	EogPluginClass parent_class;
};

/*
 * Public methods
 */
GType	eog_imgur_plugin_get_type		(void) G_GNUC_CONST;

/* All the plugins must implement this function */
G_MODULE_EXPORT GType register_eog_plugin (GTypeModule *module);

G_END_DECLS

#endif
