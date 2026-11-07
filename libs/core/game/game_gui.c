#ifndef GAME_GUI_C
#define GAME_GUI_C

#include "game.h"
#include "planet.h"
#include "game_gui.h"
#include "gdk/gdk.h"
#include "glib-object.h"
#include "glib.h"
#include "glibconfig.h"
#include "gtk/gtkcssprovider.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * This function will initialize everything to a playable state,
 * before launching the game loop, once the loop stops it will handle
 * memory deallocation as well.
 *
 * @brief Runs the full game, turn by turn, until win condition or game over is reached.
 * @todo (1) - GUI for the planet creation step instead of generating a random planet
 */
void runGUIGame(int argc, char *argv[])
{
	/* Proposed layout
	<body>
		<box> // Header - Planet Information
			<box>
				<label> // Planet Name
				<label> // Game version
			</box>
			<box>
				<box>
					<box>
						<label> // Planet Population str
						<label> // Planet Population val
					</box>
					<box>
						<label> // Planet Habitability str
						<label> // Planet Habitability val
					</box>
					<box>
						<label> // Planet Radius str
						<label> // Planet Radius val
					</box>
				</box>
				<box>
					<box>
						<label> // Planet Water str
						<label> // Planet water val
					</box>
					<box>
						<label> // Planet Wood str
						<label> // Planet Wood val
					</box>
					<box>
						<label> // Planet Minerals str
						<label> // Planet Minerals val
					</box>
				</box>
			</box>
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

	// -------------------------------------------------------------------------------------------------- //
	//                                                INIT                                                //
	// -------------------------------------------------------------------------------------------------- //

	gtk_init(&argc, &argv);

	GameUI *ui = (GameUI *)malloc(sizeof(GameUI));

	/* TODO (1) */
	Planet *planet = initPlanet(); // Random planet

	if (planet == NULL)
	{
		fprintf(stderr, "Failed to initialize planet !\n");
		exit(EXIT_FAILURE);
	}

	ui->planet = planet;

	// Styling
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

	build_header(ui, main_box);

	// ------------------
	// Middle - Event History
	// ------------------
	// The history box takes TRUE for the expand parameter so it pushes the footer to the bottom
	GtkWidget *history_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(main_box), history_box, TRUE, TRUE, 0);

	// Placeholder label for where the history list will go
	gtk_box_pack_start(GTK_BOX(history_box), gtk_label_new("[Event History Area]"), FALSE, FALSE, 0);

	// ------------------
	// Footer - Action Choice
	// ------------------

	build_footer(ui, main_box);

	// ------------------

	gtk_widget_show_all(window);

	gtk_main();

	// End of program

	printf("\n--------------------------------------------------\n"
		   "                  End of program                  \n"
		   "--------------------------------------------------\n");

	free(ui);
}

/**
 * @brief Builds the Header GUI and references the GameUI pointers for later update of values.
 * @param ui The GameUI struct that holds pointer references to labels to allow for updates
 * @param parent The parent widget.
 *
 * @todo (1) - Header Styling
 */
void build_header(GameUI *ui, GtkWidget *parent)
{
	GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	gtk_box_pack_start(GTK_BOX(parent), header_box, FALSE, FALSE, 0);

	GtkStyleContext *header_box_context = gtk_widget_get_style_context(header_box);
	gtk_style_context_add_class(header_box_context, "header");

	gchar *str_name = g_strdup_printf("%s", ui->planet->name);
	gchar *str_radius = g_strdup_printf("Planet Radius: %.2f", ui->planet->radiusKm);
	gchar *str_population = g_strdup_printf("Population: %ld", ui->planet->population);
	gchar *str_water = g_strdup_printf("Water: %d", ui->planet->water);
	gchar *str_wood = g_strdup_printf("Wood: %d", ui->planet->wood);
	gchar *str_minerals = g_strdup_printf("Minerals: %d", ui->planet->minerals);
	gchar *str_habitability = g_strdup_printf("Habitability: %d", ui->planet->habitability);

	// Labels
	ui->label_name = gtk_label_new(str_name);
	ui->label_radius = gtk_label_new(str_radius);
	ui->label_population = gtk_label_new(str_population);
	ui->label_water = gtk_label_new(str_water);
	ui->label_wood = gtk_label_new(str_wood);
	ui->label_minerals = gtk_label_new(str_minerals);
	ui->label_habitability = gtk_label_new(str_habitability);

	// Adding labels to the header
	gtk_box_pack_start(GTK_BOX(header_box), ui->label_name, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(header_box), ui->label_radius, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(header_box), ui->label_population, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(header_box), ui->label_water, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(header_box), ui->label_wood, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(header_box), ui->label_minerals, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(header_box), ui->label_habitability, FALSE, FALSE, 0);

	/* TODO (1) */
	// Styling
	GtkStyleContext *label_name_context = gtk_widget_get_style_context(ui->label_name);
	gtk_style_context_add_class(label_name_context, "planet_name");

	GtkStyleContext *label_radius_context = gtk_widget_get_style_context(ui->label_radius);
	gtk_style_context_add_class(label_radius_context, "");

	GtkStyleContext *label_population_context = gtk_widget_get_style_context(ui->label_population);
	gtk_style_context_add_class(label_population_context, "");

	GtkStyleContext *label_water_context = gtk_widget_get_style_context(ui->label_water);
	gtk_style_context_add_class(label_water_context, "");

	GtkStyleContext *label_wood_context = gtk_widget_get_style_context(ui->label_wood);
	gtk_style_context_add_class(label_wood_context, "");

	GtkStyleContext *label_minerals_context = gtk_widget_get_style_context(ui->label_minerals);
	gtk_style_context_add_class(label_minerals_context, "");

	GtkStyleContext *label_habitability_context = gtk_widget_get_style_context(ui->label_habitability);
	gtk_style_context_add_class(label_habitability_context, "");

	// Free the str now that they are in the labels.
	g_free(str_name);
	g_free(str_radius);
	g_free(str_population);
	g_free(str_water);
	g_free(str_wood);
	g_free(str_minerals);
	g_free(str_habitability);
}

/**
 * @brief Builds the Middle part GUI and references the GameUI pointers for later update of values.
 * @param ui The GameUI struct that holds pointer references to labels to allow for updates
 * @param parent The parent widget.
 */
void build_middle(GameUI *ui, GtkWidget *parent);

/**
 * @brief Builds the Footer GUI and references the GameUI pointers for later update of values.
 * @param ui The GameUI struct that holds pointer references to labels to allow for updates
 * @param parent The parent widget.
 */
void build_footer(GameUI *ui, GtkWidget *parent)
{
	GtkWidget *footer_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_pack_start(GTK_BOX(parent), footer_box, FALSE, FALSE, 0);

	// Action 1
	ui->action1_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(ui->action1_box), gtk_label_new("Action 1 Details"), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(ui->action1_box), gtk_button_new_with_label("Execute 1"), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(footer_box), ui->action1_box, TRUE, TRUE, 0);

	// Action 2
	ui->action2_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(ui->action2_box), gtk_label_new("Action 2 Details"), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(ui->action2_box), gtk_button_new_with_label("Execute 2"), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(footer_box), ui->action2_box, TRUE, TRUE, 0);

	// Action 3
	ui->action3_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(ui->action3_box), gtk_label_new("Action 3 Details"), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(ui->action3_box), gtk_button_new_with_label("Execute 3"), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(footer_box), ui->action3_box, TRUE, TRUE, 0);
}

#endif