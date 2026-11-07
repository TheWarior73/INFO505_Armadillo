#ifndef GAME_GUI_H
#define GAME_GUI_H

#include "gtk/gtk.h"
#include "planet.h"

/**
 * @brief Struct used to update important informations on the screen each turn
 */
typedef struct
{
	Planet *planet;

	// Header labels
	GtkWidget *label_name;
	GtkWidget *label_radius;
	GtkWidget *label_population;
	GtkWidget *label_water;
	GtkWidget *label_wood;
	GtkWidget *label_minerals;
	GtkWidget *label_habitability;

	// Middle section
	GtkWidget *history_log;

	// Footer buttons
	GtkWidget *action1_box;
	GtkWidget *action2_box;
	GtkWidget *action3_box;
} GameUI;

void runGUIGame(int argc, char *argv[]);

void build_header(GameUI *ui, GtkWidget *parent);
void build_middle(GameUI *ui, GtkWidget *parent);
void build_footer(GameUI *ui, GtkWidget *parent);

#endif