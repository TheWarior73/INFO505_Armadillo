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
 */
void runGUIGame(int argc, char *argv[])
{

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
	ui->main_window = window;

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
	GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	gtk_box_pack_start(GTK_BOX(parent), header_box, FALSE, FALSE, 0);

	GtkStyleContext *header_box_context = gtk_widget_get_style_context(header_box);
	gtk_style_context_add_class(header_box_context, "header");

#pragma region LABELS
	gchar *str_name = g_strdup_printf("%s", ui->planet->name);
	gchar *str_radius = g_strdup_printf("%s", format_readable_number(ui->planet->radiusKm));
	gchar *str_population = g_strdup_printf("%s", format_readable_number(ui->planet->population));
	gchar *str_water = g_strdup_printf("%s", format_readable_number(ui->planet->water));
	gchar *str_wood = g_strdup_printf("%s", format_readable_number(ui->planet->wood));
	gchar *str_minerals = g_strdup_printf("%s", format_readable_number(ui->planet->minerals));
	gchar *str_habitability = g_strdup_printf("%s", format_readable_number(ui->planet->habitability));

	// Labels
	ui->label_name = gtk_label_new(str_name);
	ui->label_radius = gtk_label_new(str_radius);
	ui->label_population = gtk_label_new(str_population);
	ui->label_water = gtk_label_new(str_water);
	ui->label_wood = gtk_label_new(str_wood);
	ui->label_minerals = gtk_label_new(str_minerals);
	ui->label_habitability = gtk_label_new(str_habitability);
#pragma endregion

#pragma region TOP BAR
	GtkWidget *top_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(header_box), top_bar, FALSE, FALSE, 0);

	// Planet Name (Left)
	gtk_widget_set_halign(ui->label_name, GTK_ALIGN_START);

	GtkStyleContext *label_name_context = gtk_widget_get_style_context(ui->label_name);
	gtk_style_context_add_class(label_name_context, "planet_name");

	// Game Version (Right)
	GtkWidget *game_version = gtk_label_new(VERSION);
	gtk_widget_set_halign(game_version, GTK_ALIGN_END);

	GtkStyleContext *version_context = gtk_widget_get_style_context(game_version);
	gtk_style_context_add_class(version_context, "game_version");

	// Pack Top Bar (Name expands to push version to the right)
	gtk_box_pack_start(GTK_BOX(top_bar), ui->label_name, TRUE, TRUE, 0);
	gtk_box_pack_end(GTK_BOX(top_bar), game_version, FALSE, FALSE, 0);
#pragma endregion

#pragma region STATS GRID
	GtkWidget *stats_grid = gtk_grid_new();
	gtk_grid_set_column_homogeneous(GTK_GRID(stats_grid), TRUE);
	gtk_grid_set_row_spacing(GTK_GRID(stats_grid), 15);
	gtk_grid_set_column_spacing(GTK_GRID(stats_grid), 15);

	GtkStyleContext *grid_context = gtk_widget_get_style_context(stats_grid);
	gtk_style_context_add_class(grid_context, "stats_grid");

	// Row 0
	gtk_grid_attach(GTK_GRID(stats_grid), create_stat_box("POPULATION", str_population, &ui->label_population, "stat_value"), 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(stats_grid), create_stat_box("HABITABILITY", str_habitability, &ui->label_habitability, "stat_value"), 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(stats_grid), create_stat_box("PLANET RADIUS", str_radius, &ui->label_radius, "stat_value"), 2, 0, 1, 1);

	// Row 1
	gtk_grid_attach(GTK_GRID(stats_grid), create_stat_box("WATER", str_water, &ui->label_water, "stat_value"), 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(stats_grid), create_stat_box("WOOD", str_wood, &ui->label_wood, "stat_value"), 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(stats_grid), create_stat_box("MINERALS", str_minerals, &ui->label_minerals, "stat_value"), 2, 1, 1, 1);

	update_stat_color(ui->label_population, ui->planet->population, MIN_POPULATION_ALLOWED * 3, MIN_POPULATION_ALLOWED * 10, 0);
	update_stat_color(ui->label_water, ui->planet->water, MIN_WATER_ALLOWED * 3, MIN_WATER_ALLOWED * 10, MAX_WATER_ALLOWED * 0.8);
	update_stat_color(ui->label_wood, ui->planet->wood, MIN_WOOD_ALLOWED * 3, MIN_WOOD_ALLOWED * 10, MAX_WOOD_ALLOWED * 0.8);
	update_stat_color(ui->label_minerals, ui->planet->minerals, MIN_MINERALS_ALLOWED * 3, MIN_MINERALS_ALLOWED * 10, 0);
	update_stat_color(ui->label_habitability, ui->planet->habitability, 25, 50, 0);
	update_stat_color(ui->label_radius, ui->planet->radiusKm, MIN_PLANET_RADIUS * 3, MIN_PLANET_RADIUS * 10, 0);

	gtk_box_pack_start(GTK_BOX(header_box), stats_grid, FALSE, FALSE, 0);
#pragma endregion

#pragma region FREE
	g_free(str_name);
	g_free(str_population);
	g_free(str_habitability);
	g_free(str_radius);
	g_free(str_water);
	g_free(str_wood);
	g_free(str_minerals);
#pragma endregion
}

/**
 * @brief Builds the Middle part GUI and references the GameUI pointers for later update of values.
 * @param ui The GameUI struct that holds pointer references to labels to allow for updates
 * @param parent The parent widget.
 */
void build_middle(GameUI *ui, GtkWidget *parent)
{
	GtkWidget *history_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *title = gtk_label_new("Log History");
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
 * @todo (3) - Footer Styling
 */
void build_footer(GameUI *ui, GtkWidget *parent)
{
	GtkWidget *footer_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_pack_start(GTK_BOX(parent), footer_box, FALSE, FALSE, 0);

#pragma region Buttons

	gchar *action1_btn_str = format_change(ui->action1);
	gchar *action2_btn_str = format_change(ui->action2);
	gchar *action3_btn_str = format_change(ui->action3);

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

	GtkStyleContext *action1_ctx = gtk_widget_get_style_context(ui->action1_box);
	gtk_style_context_add_class(action1_ctx, "action_box");

	GtkStyleContext *action2_ctx = gtk_widget_get_style_context(ui->action2_box);
	gtk_style_context_add_class(action2_ctx, "action_box");

	GtkStyleContext *action3_ctx = gtk_widget_get_style_context(ui->action3_box);
	gtk_style_context_add_class(action3_ctx, "action_box");
#pragma endregion
}

/**
 * @brief Handles the action when clicked.
 * Handles the action button click, apply the changes to the planet and starts a new game turn.
 * @param button a GtkWidget pointer that should represent a button.
 * @param data the data provided by the callback, it should be of type GameUI *
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
			break;
		};
		case 2: {
			elt = ui->action2;
			break;
		};
		case 3: {
			elt = ui->action3;
			break;
		};
		case -1: {
			fprintf(stderr, "ERROR: on_action_clicked - Invalid pointer to a button.\n");
			return;
		}
	}

	// -- NEW TURN --

	Element *random_event = drawRandomElement(ui->planet->Events);
	gameTurn(ui->planet, random_event, elt);

	// Update Actions
	ui->action1 = drawRandomElement(ui->planet->Actions);
	ui->action2 = drawRandomElement(ui->planet->Actions);
	ui->action3 = drawRandomElement(ui->planet->Actions);

	update_display(ui);

	// Check win/loose conditions :

	if (planetHasLost(ui->planet))
	{
		show_game_popup(GTK_WINDOW(ui->main_window),
						GTK_MESSAGE_ERROR,
						"SYSTEM FAILURE",
						"Critical failure detected.\nAll life on the planet has perished. Game Over.");

		gtk_widget_destroy(ui->main_window);
	}
	else if (planetHasWon(ui->planet))
	{
		show_game_popup(GTK_WINDOW(ui->main_window),
						GTK_MESSAGE_INFO,
						"MISSION SUCCESS",
						"Planet stabilized.\nPopulation targets reached. You win!");

		gtk_widget_destroy(ui->main_window);
	}
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
 * @todo (2) - history
 */
void update_display(GameUI *ui)
{
#pragma region Header
	// Str
	gchar *new_str_population = g_strdup_printf("%s", format_readable_number(ui->planet->population));
	gchar *new_str_water = g_strdup_printf("%s", format_readable_number(ui->planet->water));
	gchar *new_str_wood = g_strdup_printf("%s", format_readable_number(ui->planet->wood));
	gchar *new_str_minerals = g_strdup_printf("%s", format_readable_number(ui->planet->minerals));
	gchar *new_str_habitability = g_strdup_printf("%s", format_readable_number(ui->planet->habitability));
	gchar *new_str_radius = g_strdup_printf("%s", format_readable_number(ui->planet->radiusKm));

	// Labels
	gtk_label_set_text(GTK_LABEL(ui->label_population), new_str_population);
	gtk_label_set_text(GTK_LABEL(ui->label_water), new_str_water);
	gtk_label_set_text(GTK_LABEL(ui->label_wood), new_str_wood);
	gtk_label_set_text(GTK_LABEL(ui->label_minerals), new_str_minerals);
	gtk_label_set_text(GTK_LABEL(ui->label_habitability), new_str_habitability);
	gtk_label_set_text(GTK_LABEL(ui->label_radius), new_str_radius);

	update_stat_color(ui->label_population, ui->planet->population, MIN_POPULATION_ALLOWED * 3, MIN_POPULATION_ALLOWED * 10, 0);
	update_stat_color(ui->label_water, ui->planet->water, MIN_WATER_ALLOWED * 3, MIN_WATER_ALLOWED * 10, MAX_WATER_ALLOWED * 0.8);
	update_stat_color(ui->label_wood, ui->planet->wood, MIN_WOOD_ALLOWED * 3, MIN_WOOD_ALLOWED * 10, MAX_WOOD_ALLOWED * 0.8);
	update_stat_color(ui->label_minerals, ui->planet->minerals, MIN_MINERALS_ALLOWED * 3, MIN_MINERALS_ALLOWED * 10, 0);
	update_stat_color(ui->label_habitability, ui->planet->habitability, 25, 50, 0);
	update_stat_color(ui->label_radius, ui->planet->radiusKm, MIN_PLANET_RADIUS * 3, MIN_PLANET_RADIUS * 10, 0);

	// Free
	g_free(new_str_population);
	g_free(new_str_water);
	g_free(new_str_wood);
	g_free(new_str_minerals);
	g_free(new_str_habitability);
	g_free(new_str_radius);
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

	gchar *new_action1_btn_str = format_change(ui->action1);
	gchar *new_action2_btn_str = format_change(ui->action2);
	gchar *new_action3_btn_str = format_change(ui->action3);

	// Labels
	gtk_label_set_text(GTK_LABEL(ui->action1_label), new_str_name_element_1);
	gtk_label_set_text(GTK_LABEL(ui->action2_label), new_str_name_element_2);
	gtk_label_set_text(GTK_LABEL(ui->action3_label), new_str_name_element_3);

	gtk_button_set_label(GTK_BUTTON(ui->action1_btn), new_action1_btn_str);
	gtk_button_set_label(GTK_BUTTON(ui->action2_btn), new_action2_btn_str);
	gtk_button_set_label(GTK_BUTTON(ui->action3_btn), new_action3_btn_str);

	// Free
	g_free(new_str_name_element_1);
	g_free(new_str_name_element_2);
	g_free(new_str_name_element_3);

	g_free(new_action1_btn_str);
	g_free(new_action2_btn_str);
	g_free(new_action3_btn_str);
#pragma endregion
}

/**
 * @brief returns a gchar that have the data to display a change (which type, what quantity).
 * @param action The action which is going to be looked at.
 * @return a gchar * (gtk char*) that holds the changed target name & value.
 */
gchar *format_change(Element *action)
{
	if (action == NULL)
	{
		return g_strdup("Empty Action");
	}

	switch (action->changeTarget)
	{
		case ELEMENT_CHANGE_NONE:
			return g_strdup("0");

		case ELEMENT_CHANGE_POPULATION:
			return g_strdup_printf("%s: %s",
								   strOfElementChangeTarget(action->changeTarget),
								   format_readable_delta(action->change.population));

		case ELEMENT_CHANGE_RADIUS:
			return g_strdup_printf("%s: %s",
								   strOfElementChangeTarget(action->changeTarget),
								   format_readable_delta(action->change.radiusKm));

		case ELEMENT_CHANGE_WATER:
		case ELEMENT_CHANGE_MINERALS:
		case ELEMENT_CHANGE_WOOD:
		case ELEMENT_CHANGE_HABITABILITY:
			return g_strdup_printf("%s: %s",
								   strOfElementChangeTarget(action->changeTarget),
								   format_readable_delta(action->change.quantity));

		default:
			return g_strdup("0");
	}
}

/**
 * @brief Helper function to create a vertically stacked stat box.
 * Assigns the dynamically changing value label to the provided pointer address.
 */
GtkWidget *create_stat_box(const gchar *title_text, const gchar *value_text, GtkWidget **out_value_label, const gchar *css_class)
{
	GtkWidget *stat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

	// Labels
	GtkWidget *title_label = gtk_label_new(title_text);
	*out_value_label = gtk_label_new(value_text);

	// Left-align the text
	gtk_widget_set_halign(title_label, GTK_ALIGN_START);
	gtk_widget_set_halign(*out_value_label, GTK_ALIGN_START);

	// Styling
	GtkStyleContext *box_ctx = gtk_widget_get_style_context(stat_box);
	gtk_style_context_add_class(box_ctx, "stat_box");

	GtkStyleContext *title_ctx = gtk_widget_get_style_context(title_label);
	gtk_style_context_add_class(title_ctx, "stat_title");

	GtkStyleContext *val_ctx = gtk_widget_get_style_context(*out_value_label);
	gtk_style_context_add_class(val_ctx, "stat_value");
	if (css_class != NULL)
	{
		gtk_style_context_add_class(val_ctx, css_class);
	}

	// Add to box
	gtk_box_pack_start(GTK_BOX(stat_box), title_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(stat_box), *out_value_label, FALSE, FALSE, 0);

	return stat_box;
}

/**
 * @brief Displays a popup message dialog to the user.
 *
 * @param parent_window The main application window
 * @param type The type of message
 * @param title The title of the popup window
 * @param message The main text to display
 */
void show_game_popup(GtkWindow *parent_window, GtkMessageType type, const gchar *title, const gchar *message)
{
	GtkWidget *dialog = gtk_message_dialog_new(parent_window,
											   GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
											   type,
											   GTK_BUTTONS_OK,
											   "%s",
											   message);

	gtk_window_set_title(GTK_WINDOW(dialog), title);

	// Optional: Add a custom CSS class if you want to style the popup to match your retro theme
	GtkStyleContext *ctx = gtk_widget_get_style_context(dialog);
	gtk_style_context_add_class(ctx, "game_popup");

	// Block until the user clicks OK
	gtk_dialog_run(GTK_DIALOG(dialog));

	// Cleanup the popup
	gtk_widget_destroy(dialog);
}

/**
 * @brief Formats large numbers into readable K / M / B strings.
 * @param val the number to be formated
 * Examples: 950 -> "950", 45200 -> "45.2K", 1822667 -> "1.82M"
 */
gchar *format_readable_number(long val)
{
	long abs_val = labs(val);
	const char *sign = (val < 0) ? "-" : "";

	if (abs_val >= 1000000000L)
	{
		return g_strdup_printf("%s%.2fB", sign, abs_val / 1000000000.0);
	}
	else if (abs_val >= 1000000L)
	{
		return g_strdup_printf("%s%.2fM", sign, abs_val / 1000000.0);
	}
	else if (abs_val >= 1000L)
	{
		return g_strdup_printf("%s%.1fK", sign, abs_val / 1000.0);
	}
	return g_strdup_printf("%ld", val);
}

/**
 * @brief Formats signed deltas for action buttons
 * @param val the number to be formated
 * Example: +50.0K, -1.20M
 */
gchar *format_readable_delta(long val)
{
	long abs_val = labs(val);
	const char *sign = (val >= 0) ? "+" : "-";

	if (abs_val >= 1000000000L)
	{
		return g_strdup_printf("%s%.2fB", sign, abs_val / 1000000000.0);
	}
	else if (abs_val >= 1000000L)
	{
		return g_strdup_printf("%s%.2fM", sign, abs_val / 1000000.0);
	}
	else if (abs_val >= 1000L)
	{
		return g_strdup_printf("%s%.1fK", sign, abs_val / 1000.0);
	}
	return g_strdup_printf("%+ld", val);
}

void update_stat_color(GtkWidget *label, long value, long danger_low, long warn_low, long warn_high)
{
	GtkStyleContext *ctx = gtk_widget_get_style_context(label);

	// Clear states
	gtk_style_context_remove_class(ctx, "stat_critical");
	gtk_style_context_remove_class(ctx, "stat_warning");
	gtk_style_context_remove_class(ctx, "stat_normal");
	gtk_style_context_remove_class(ctx, "stat_optimal");

	if (value <= danger_low || (warn_high > 0 && value >= warn_high))
	{
		gtk_style_context_add_class(ctx, "stat_critical");
	}
	else if (value <= warn_low)
	{
		gtk_style_context_add_class(ctx, "stat_warning");
	}
	else if (value >= 1000000L)
	{
		gtk_style_context_add_class(ctx, "stat_optimal");
	}
	else
	{
		gtk_style_context_add_class(ctx, "stat_normal");
	}
}

#endif