#ifndef GAME_GUI_H
#define GAME_GUI_H

#include "element.h"
#include "gtk/gtk.h"
#include "planet.h"

/**
 * @brief Struct used to update important informations on the screen each turn
 */
typedef struct
{
	/** @brief The game planet */
	Planet *planet;

	// ---------------
	//
	//
	// Header labels
	/** @brief The planet name label */
	GtkWidget *label_name;

	/** @brief The planet radius label */
	GtkWidget *label_radius;

	/** @brief The planet population label */
	GtkWidget *label_population;

	/** @brief The planet water label */
	GtkWidget *label_water;

	/** @brief The planet wood label */
	GtkWidget *label_wood;

	/** @brief The planet minerals label */
	GtkWidget *label_minerals;

	/** @brief The planet habitability label */
	GtkWidget *label_habitability;

	// ---------------
	//
	//
	// Middle section
	/** @brief The game's history logs (Events & Actions) */
	GtkWidget *history_log;

	// ---------------
	//
	//
	// Footer
	/** @brief The First Action Box (left) */
	GtkWidget *action1_box;
	/** @brief The First Action Button (left) */
	GtkWidget *action1_btn;
	/** @brief The First Action Button (left) */
	GtkWidget *action1_label;
	/** @brief The First Action (left) */
	Element *action1;

	/** @brief The Second Action Box (middle) */
	GtkWidget *action2_box;
	/** @brief The Second Action Button (middle) */
	GtkWidget *action2_btn;
	/** @brief The Second Action Button (middle) */
	GtkWidget *action2_label;
	/** @brief The Second Action (middle) */
	Element *action2;

	/** @brief The Third Action Box (right) */
	GtkWidget *action3_box;
	/** @brief The Third Action Button (right) */
	GtkWidget *action3_btn;
	/** @brief The Third Action Button (right) */
	GtkWidget *action3_label;
	/** @brief The Third Action (right) */
	Element *action3;
} GameUI;

void runGUIGame(int argc, char *argv[]);

void build_header(GameUI *ui, GtkWidget *parent);
void build_middle(GameUI *ui, GtkWidget *parent);
void build_footer(GameUI *ui, GtkWidget *parent);

void on_action_clicked(GtkWidget *button, gpointer data);
void on_window_destroy(GtkWidget *widget, gpointer data);

// Helpers

int get_clicked_button(GtkWidget *button, GameUI *ui);
void update_display(GameUI *ui);
gchar *format_change(Element *action);

#endif