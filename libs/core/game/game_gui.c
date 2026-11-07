#ifndef GAME_GUI_C
#define GAME_GUI_C

#include "game.h"
#include "gdk/gdk.h"
#include "glib-object.h"
#include "glib.h"
#include "glibconfig.h"
#include "gtk/gtkcssprovider.h"
#include "planet.h"
#include <gtk/gtk.h>

/**
 * This function will initialize everything to a playable state,
 * before launching the game loop, once the loop stops it will handle
 * memory deallocation as well.
 *
 * @brief Runs the full game, turn by turn, until win condition or game over is reached.
 */
void runGUIGame(int argc, char *argv[])
{
	/*
	<body>
		<box> // Header - Planet Information
			<label> // Planet Name
			<label> // Planet Radius
			<label> // Planet Population
			<label> // Planet Water
			<label> // Planet Wood
			<label> // Planet Minerals
			<label> // Planet Habitability
		</box>
		<box> // Event History

		</box>
		<box> // Footer	- Action Choice
			<box> // Action 1
				<label>
				<button>
			</box>
			<box> // Action 2
				<label>
				<button>
			</box>
			<box> // Action 3
				<label>
				<button>
			</box>
		</box>
	</body>
	*/

	gtk_init(&argc, &argv);

	// ----------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------

	GtkCssProvider *cssProvider = gtk_css_provider_new();

	gtk_css_provider_load_from_path(cssProvider, "./style/main.css", NULL);

	gtk_style_context_add_provider_for_screen(
		gdk_screen_get_default(),
		GTK_STYLE_PROVIDER(cssProvider),
		GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	g_object_unref(cssProvider); // Dispose of the css Provider as the screen now holds it's data.

	// ----------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------

	// New window
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	// Title & size
	gtk_window_set_title(GTK_WINDOW(window), "The Armadillo Project");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

	// Subscribing to the destroy action
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// ------------------
	// Main Container
	// ------------------
	GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_container_add(GTK_CONTAINER(window), main_box);

	GtkStyleContext *box_context = gtk_widget_get_style_context(main_box);
	gtk_style_context_add_class(box_context, "main_wrapper");

	// ------------------
	// Header - Planet Information
	// ------------------
	GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	gtk_box_pack_start(GTK_BOX(main_box), header_box, FALSE, FALSE, 0);

	GtkStyleContext *header_box_context = gtk_widget_get_style_context(header_box);
	gtk_style_context_add_class(header_box_context, "header");

	/*
		//////////////////////////////////////////////////////////////////////////////////////////
												Planet Init
	*/

	/** @todo GUI for the planet creation step instead of generating a random planet */
	Planet *planet = initPlanet(); // Random planet

	if (planet == NULL)
	{
		g_print("Failed to initialize planet !");
	}

	gchar *str_name = g_strdup_printf("%s", planet->name);
	gchar *str_radius = g_strdup_printf("Planet Radius: %f", planet->radiusKm);
	gchar *str_population = g_strdup_printf("Population: %ld", planet->population);
	gchar *str_water = g_strdup_printf("Water: %d", planet->water);
	gchar *str_wood = g_strdup_printf("Wood: %d", planet->wood);
	gchar *str_minerals = g_strdup_printf("Minerals: %d", planet->minerals);
	gchar *str_habitability = g_strdup_printf("Habitability: %d", planet->habitability);

	/*
		//////////////////////////////////////////////////////////////////////////////////////////
	*/

	gtk_box_pack_start(GTK_BOX(header_box), gtk_label_new(str_name), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(header_box), gtk_label_new(str_radius), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(header_box), gtk_label_new(str_population), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(header_box), gtk_label_new(str_water), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(header_box), gtk_label_new(str_wood), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(header_box), gtk_label_new(str_minerals), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(header_box), gtk_label_new(str_habitability), FALSE, FALSE, 0);

	// Free the str now that they are in the labels.
	g_free(str_name);
	g_free(str_radius);
	g_free(str_population);
	g_free(str_water);
	g_free(str_wood);
	g_free(str_minerals);
	g_free(str_habitability);

	// ------------------
	// Middle - Event History
	// ------------------
	// The history box takes TRUE for the expand parameter so it pushes the footer to the bottom
	GtkWidget *history_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(main_box), history_box, TRUE, TRUE, 0);

	// Placeholder label for where your history text/list will go
	gtk_box_pack_start(GTK_BOX(history_box), gtk_label_new("[Event History Area]"), FALSE, FALSE, 0);

	// ------------------
	// Footer - Action Choice
	// ------------------
	GtkWidget *footer_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_pack_start(GTK_BOX(main_box), footer_box, FALSE, FALSE, 0);

	// Action 1
	GtkWidget *action1_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(action1_box), gtk_label_new("Action 1 Details"), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(action1_box), gtk_button_new_with_label("Execute 1"), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(footer_box), action1_box, TRUE, TRUE, 0);

	// Action 2
	GtkWidget *action2_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(action2_box), gtk_label_new("Action 2 Details"), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(action2_box), gtk_button_new_with_label("Execute 2"), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(footer_box), action2_box, TRUE, TRUE, 0);

	// Action 3
	GtkWidget *action3_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(action3_box), gtk_label_new("Action 3 Details"), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(action3_box), gtk_button_new_with_label("Execute 3"), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(footer_box), action3_box, TRUE, TRUE, 0);

	// ------------------

	gtk_widget_show_all(window);

	gtk_main();
}

#endif