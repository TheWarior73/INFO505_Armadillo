#ifndef GAME_GUI_C
#define GAME_GUI_C

#include "element.h"
#include "game.h"
#include "planet.h"
#include "game_gui.h"
#include "gdk/gdk.h"
#include "glib-object.h"
#include "glib.h"
#include "glibconfig.h"
#include "gtk/gtkcssprovider.h"
#include <asm-generic/errno-base.h>
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
 * @todo (2) - Move the middle component initialisation logic to build_middle.
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

	GameUI *ui = g_new0(GameUI, 1);

	/* TODO (1) */
	ui->planet = initPlanet(); // Random planet

	if (ui->planet == NULL)
	{
		fprintf(stderr, "Failed to initialize planet !\n");
		exit(EXIT_FAILURE);
	}

	// Initialising the first batch of actions.
	ui->action1 = drawRandomElement(ui->planet->Actions);
	ui->action2 = drawRandomElement(ui->planet->Actions);
	ui->action3 = drawRandomElement(ui->planet->Actions);

	printf("%s\n", ui->action1->nameElement);

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
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 500);

	// ------------------

	// Main Container
	GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_container_add(GTK_CONTAINER(window), main_box);

	GtkStyleContext *box_context = gtk_widget_get_style_context(main_box);
	gtk_style_context_add_class(box_context, "main_wrapper");

	// ------------------

	// Header - Planet Information
	build_header(ui, main_box);

	// Middle - Event and action history
	build_middle(ui, main_box);

	// Footer - Action Choice
	build_footer(ui, main_box);

	// ------------------

	// Subscriptions
	g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), ui);
	g_signal_connect(ui->action1_btn, "clicked", G_CALLBACK(on_action_clicked), ui);
	g_signal_connect(ui->action2_btn, "clicked", G_CALLBACK(on_action_clicked), ui);
	g_signal_connect(ui->action3_btn, "clicked", G_CALLBACK(on_action_clicked), ui);

	// -------------------

	gtk_widget_show_all(window);
	gtk_main();

	// End of program

	printf("\n--------------------------------------------------\n"
		   "                  End of program                  \n"
		   "--------------------------------------------------\n");
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

#pragma region LABELS
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
#pragma endregion

#pragma region STYLING
	/* TODO (1) */
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
#pragma endregion

#pragma region FREE
	// Free the str now that they are in the labels.
	g_free(str_name);
	g_free(str_radius);
	g_free(str_population);
	g_free(str_water);
	g_free(str_wood);
	g_free(str_minerals);
	g_free(str_habitability);
#pragma endregion
}

/**
 * @brief Builds the Middle part GUI and references the GameUI pointers for later update of values.
 * @param ui The GameUI struct that holds pointer references to labels to allow for updates
 * @param parent The parent widget.
 *
 * @todo (1) - Middle Component Building
 */
void build_middle(GameUI *ui, GtkWidget *parent)
{
	GtkWidget *history_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *title = gtk_label_new("Historique des choix précédents");
	GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	GtkTextBuffer *buffer;
	gchar *history = NULL;
	GError *error = NULL;

	gtk_box_pack_start(GTK_BOX(parent), history_box, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(history_box), title, FALSE, FALSE, 0);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
								   GTK_POLICY_AUTOMATIC,
								   GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start(GTK_BOX(history_box), scrolled_window, TRUE, TRUE, 0);

	ui->history_log = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(ui->history_log), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(ui->history_log), FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(ui->history_log), GTK_WRAP_WORD_CHAR);
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ui->history_log));
	gtk_container_add(GTK_CONTAINER(scrolled_window), ui->history_log);

	if (g_file_get_contents(GAME_HISTORY_FILE, &history, NULL, &error) && history[0] != '\0')
	{
		gtk_text_buffer_set_text(buffer, history, -1);
	}
	else
	{
		gtk_text_buffer_set_text(buffer, "Aucun choix précédent enregistré.", -1);
	}
	g_free(history);
	g_clear_error(&error);
}

/**
 * @brief Builds the Footer GUI and references the GameUI pointers for later update of values.
 * @param ui The GameUI struct that holds pointer references to labels to allow for updates
 * @param parent The parent widget.
 *
 * @todo (1) - Buttons Structure
 * @todo (2) - Buttons Callbacks Subscription
 * @todo (3) - Footer Styling
 */
void build_footer(GameUI *ui, GtkWidget *parent)
{
	GtkWidget *footer_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_pack_start(GTK_BOX(parent), footer_box, FALSE, FALSE, 0);

#pragma region Buttons

	gchar *action1_btn_str = ui->action1->nameElement;
	gchar *action2_btn_str = ui->action2->nameElement;
	gchar *action3_btn_str = ui->action3->nameElement;

	ui->action1_btn = gtk_button_new_with_label(action1_btn_str);
	ui->action2_btn = gtk_button_new_with_label(action2_btn_str);
	ui->action3_btn = gtk_button_new_with_label(action3_btn_str);
#pragma endregion

#pragma region Labels
	// Str
	gchar *str_box_nb1 = g_strdup_printf("01");
	gchar *str_box_nb2 = g_strdup_printf("02");
	gchar *str_box_nb3 = g_strdup_printf("03");

	gchar *str_name_element_1 = g_strdup_printf("%s", ui->action1->nameElement);
	gchar *str_name_element_2 = g_strdup_printf("%s", ui->action2->nameElement);
	gchar *str_name_element_3 = g_strdup_printf("%s", ui->action3->nameElement);

	// labels building
	GtkWidget *box_nb1 = gtk_label_new(str_box_nb1);
	GtkWidget *box_nb2 = gtk_label_new(str_box_nb2);
	GtkWidget *box_nb3 = gtk_label_new(str_box_nb3);

	ui->action1_label = gtk_label_new(str_name_element_1);
	ui->action2_label = gtk_label_new(str_name_element_2);
	ui->action3_label = gtk_label_new(str_name_element_3);

	// Free
	g_free(str_box_nb1);
	g_free(str_box_nb2);
	g_free(str_box_nb3);

	g_free(str_name_element_1);
	g_free(str_name_element_2);
	g_free(str_name_element_3);

#pragma endregion

#pragma region Building
	ui->action1_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	gtk_box_pack_start(GTK_BOX(ui->action1_box), box_nb1, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(ui->action1_box), ui->action1_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(ui->action1_box), ui->action1_btn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(footer_box), ui->action1_box, TRUE, TRUE, 0);

	ui->action2_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(ui->action2_box), box_nb2, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(ui->action2_box), ui->action2_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(ui->action2_box), ui->action2_btn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(footer_box), ui->action2_box, TRUE, TRUE, 0);

	ui->action3_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(ui->action3_box), box_nb3, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(ui->action3_box), ui->action3_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(ui->action3_box), ui->action3_btn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(footer_box), ui->action3_box, TRUE, TRUE, 0);
#pragma endregion
}

/**
 * @brief Handles the action when clicked.
 * Handles the action button click, apply the changes to the planet and starts a new game turn.
 * @param button a GtkWidget pointer that should represent a button.
 * @param data the data provided by the callback, it should be of type GameUI *
 * @todo (1) - fix header update
 */
void on_action_clicked(GtkWidget *button, gpointer data)
{
	GameUI *ui = (GameUI *)data;
	int clicked_button = get_clicked_button(button, ui);
	Element *elt = NULL;

	switch (clicked_button)
	{
		case 1: {
			elt = ui->action1;
			printf("1 %s\n", elt->nameElement);
			break;
		};
		case 2: {
			elt = ui->action2;
			printf("2 %s\n", elt->nameElement);
			break;
		};
		case 3: {
			elt = ui->action3;
			printf("3: %s\n", elt->nameElement);
			break;
		};
		case -1: {
			fprintf(stderr, "ERROR: on_action_clicked - Invalid pointer to a button.\n");
			return;
		}
	}

	// Apply the action
	int actionRes = applyElementChange(ui->planet, elt);

	if (actionRes == 0)
	{
		fprintf(stderr, "Action.applyElementChange failed: %50s\n", elt->nameElement);
	}

	update_display(ui);

	// -- NEW TURN --

	Element *random_event = drawRandomElement(ui->planet->Events);

	int eltRes = applyElementChange(ui->planet, random_event);

	if (eltRes == 0)
	{
		fprintf(stderr, "Element.applyElementChange failed: %50s\n", random_event->nameElement);
	}

	// Add to history
	FILE *history = fopen(GAME_HISTORY_FILE, "a");
	if (history != NULL)
	{
		fprintf(history, "Event: %s | Action: %s\n", random_event != NULL ? random_event->nameElement : "None", elt != NULL ? elt->nameElement : "None");
		fclose(history);
	}
	else
	{
		perror("Unable to append game history");
	}

	// Update Actions
	ui->action1 = drawRandomElement(ui->planet->Actions);
	ui->action2 = drawRandomElement(ui->planet->Actions);
	ui->action3 = drawRandomElement(ui->planet->Actions);

	update_display(ui);
}

/**
 * @brief Callback to the signal of window destroy. Properly disposes of elements in memory.
 * Destroys the planet and the GameUI by freeing the memory, then calls gtk_main_quit() to exit the program.
 * @param widget a GtkWidget (probably the main window)
 * @param data the data provided by the callback, it should be of type GameUI *
 */
void on_window_destroy(GtkWidget *widget, gpointer data)
{
	GameUI *ui = (GameUI *)data;
	destroyPlanet(ui->planet);
	g_free(ui);
	gtk_main_quit();

	// So the widget is not unused.
	g_free(widget);
}

// --- Helpers ---

/**
 * @brief Obtains the button number that was clicked.
 * @returns -1 in case the elements do not match, otherwise the button number {1, 2, 3}
 *
 * @param button a pointer to a GtkWidget, which is supposed to be a button.
 * @param ui the GameUI, it holds a pointer too each action buttons in the program
 */
int get_clicked_button(GtkWidget *button, GameUI *ui)
{
	if (button == ui->action1_btn)
	{
		return 1;
	}
	else if (button == ui->action2_btn)
	{
		return 2;
	}
	else if (button == ui->action3_btn)
	{
		return 3;
	}
	else
	{
		return -1;
	}
}

/**
 * @brief updates the program's display.
 * @param ui Holds the relevant pointers to labels to update them accordingly.
 * Updates the labels that hold dynamic data: i.e water as well as the buttons.
 * @todo (1) - fix header update
 * @todo (2) - history
 * @todo (3) - header btn update
 */
void update_display(GameUI *ui)
{
#pragma region Header
	// Str
	gchar *new_str_population = g_strdup_printf("Population: %ld", ui->planet->population);
	gchar *new_str_water = g_strdup_printf("Water: %d", ui->planet->water);
	gchar *new_str_wood = g_strdup_printf("Wood: %d", ui->planet->wood);
	gchar *new_str_minerals = g_strdup_printf("Minerals: %d", ui->planet->minerals);
	gchar *new_str_habitability = g_strdup_printf("Habitability: %d", ui->planet->habitability);

	// Labels
	gtk_label_set_text(GTK_LABEL(ui->label_population), new_str_population);
	gtk_label_set_text(GTK_LABEL(ui->label_water), new_str_water);
	gtk_label_set_text(GTK_LABEL(ui->label_wood), new_str_wood);
	gtk_label_set_text(GTK_LABEL(ui->label_minerals), new_str_minerals);
	gtk_label_set_text(GTK_LABEL(ui->label_habitability), new_str_habitability);

	// Free
	g_free(new_str_population);
	g_free(new_str_water);
	g_free(new_str_wood);
	g_free(new_str_minerals);
	g_free(new_str_habitability);
#pragma endregion

#pragma region History
	// Str

	// Labels

	// Free

#pragma endregion

#pragma region Footer
	// Str
	gchar *new_str_name_element_1 = g_strdup_printf("%s", ui->action1->nameElement);
	gchar *new_str_name_element_2 = g_strdup_printf("%s", ui->action2->nameElement);
	gchar *new_str_name_element_3 = g_strdup_printf("%s", ui->action3->nameElement);

	// Labels
	gtk_label_set_text(GTK_LABEL(ui->action1_label), new_str_name_element_1);
	gtk_label_set_text(GTK_LABEL(ui->action2_label), new_str_name_element_2);
	gtk_label_set_text(GTK_LABEL(ui->action3_label), new_str_name_element_3);

	// Free
	g_free(new_str_name_element_1);
	g_free(new_str_name_element_2);
	g_free(new_str_name_element_3);
#pragma endregion
}

#endif