#ifndef GAME_GUI_H
#define GAME_GUI_H

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
	// Footer buttons
	/** @brief The First Action Box (left) */
	GtkWidget *action1_box;
	GtkWidget *action1_btn;

	/** @brief The Second Action Box (middle) */
	GtkWidget *action2_box;
	GtkWidget *action2_btn;

	/** @brief The Third Action Box (right) */
	GtkWidget *action3_box;
	GtkWidget *action3_btn;
} GameUI;

void runGUIGame(int argc, char *argv[]);

void build_header(GameUI *ui, GtkWidget *parent);
void build_middle(GameUI *ui, GtkWidget *parent);
void build_footer(GameUI *ui, GtkWidget *parent);

void on_action1_clicked(GtkWidget *button, gpointer data);
void on_action2_clicked(GtkWidget *button, gpointer data);
void on_action3_clicked(GtkWidget *button, gpointer data);
void on_window_destroy(GtkWidget *widget, gpointer data);

#endif