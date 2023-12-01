/***************************************************************************
 *   Author Alan Crispin                                                   *
 *   crispinalan@gmail.com                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation.                                         *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

/*
 * Use MAKEFILE to compile
 *
*/


#include <gtk/gtk.h>
#include <libnotify/notify.h>

//for libnotify you need to install 
//sudo apt install libnotify-dev

//use make to build

//---------------------------------------------------------------
// notifications
//----------------------------------------------------------------

static void button_send_clicked(GtkButton *button, gpointer   user_data)
{
	
	GtkWindow *window =user_data;   	
	//GtkWidget *app = g_object_get_data(G_OBJECT(window), "window-app-key");	
	
	GtkEntryBuffer *buffer_title;
	GtkWidget *entry_title = g_object_get_data(G_OBJECT(button), "button-title-key");
	buffer_title = gtk_entry_get_buffer(GTK_ENTRY(entry_title));
	const gchar* title = gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(buffer_title));
	
	GtkEntryBuffer *buffer_body;
	GtkWidget *entry_body = g_object_get_data(G_OBJECT(button), "button-body-key");
	buffer_body = gtk_entry_get_buffer(GTK_ENTRY(entry_body));
	const gchar* body = gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(buffer_body));
	
	GtkWidget *check_button_gnotification = g_object_get_data(G_OBJECT(button), "check-button-gnotification-key");	
	int is_gnotification = gtk_check_button_get_active(GTK_CHECK_BUTTON(check_button_gnotification));
	
	if(is_gnotification) {
	g_print("Sending using GNotification\n");
	GNotification *notification = g_notification_new (title);	
	g_notification_set_body (notification,body);	
	GIcon *icon = g_themed_icon_new ("dialog-information");
	g_notification_set_icon (notification, icon);
	g_application_send_notification (g_application_get_default(), NULL, notification);
	//g_application_send_notification(G_APPLICATION(app), NULL, notification);	
	g_object_unref (icon);
	g_object_unref (notification);
	} //if
	else {
	g_print("Sending using libnotify\n");
	NotifyNotification *notify;
	notify_init("notification"); 
	notify = notify_notification_new(title, body, NULL);
	
	//notify_notification_set_category (notify, "GTimeUtils");
	//NOTIFY_URGENCY
	//CRITICAL - Critical urgency.
	//LOW - Low urgency.
	//NORMAL - Normal urgency.
	notify_notification_set_urgency (notify, NOTIFY_URGENCY_NORMAL);
	notify_notification_show(notify,NULL);	 
	}  //else  

}

static void activate (GtkApplication* app, gpointer user_data)
{
	GtkWidget *window;	
	GtkWidget *button_send;
	GtkWidget *box;	
	GtkWidget *label_entry_title;
	GtkWidget *entry_title;
	GtkWidget *label_body;
	GtkWidget *entry_body;
	// Check buttons
	GtkWidget *check_button_gnotification;
			
	//setup window
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Notification Tester");
	gtk_window_set_default_size (GTK_WINDOW (window),400, 180);	
	box =gtk_box_new(GTK_ORIENTATION_VERTICAL,1); 
    gtk_window_set_child (GTK_WINDOW (window), box);
	
	//setup title	
	label_entry_title = gtk_label_new("Title");
	entry_title = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry_title), 100);
	GtkEntryBuffer *buffer_title;
	buffer_title = gtk_entry_get_buffer(GTK_ENTRY(entry_title));	
	gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(buffer_title),"Title Text",-1);
 
 	//setup body
	label_body = gtk_label_new("Body");
	entry_body = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry_body), 150);
	GtkEntryBuffer *buffer_body;
	buffer_body = gtk_entry_get_buffer(GTK_ENTRY(entry_body));	
	gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(buffer_body),"Body Text",-1);
	
	// check button
	check_button_gnotification = gtk_check_button_new_with_label("Use GNotification");
			
	//create button send
	button_send = gtk_button_new_with_label ("Send Notification");
	g_signal_connect (GTK_BUTTON (button_send),"clicked", G_CALLBACK (button_send_clicked), G_OBJECT (window));
		
	gtk_box_append(GTK_BOX(box), label_entry_title);
	gtk_box_append(GTK_BOX(box), entry_title);	
	gtk_box_append(GTK_BOX(box), label_body);
	gtk_box_append(GTK_BOX(box), entry_body);
	gtk_box_append(GTK_BOX(box), check_button_gnotification);	
	gtk_box_append(GTK_BOX(box), button_send);	
		
	g_object_set_data(G_OBJECT(button_send), "button-title-key", entry_title);
	g_object_set_data(G_OBJECT(button_send), "button-body-key", entry_body);
	g_object_set_data(G_OBJECT(button_send), "check-button-gnotification-key", check_button_gnotification);
		
	//g_object_set_data(G_OBJECT(window), "window-app-key",app);
	gtk_window_present (GTK_WINDOW (window)); 
		
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;
  app = gtk_application_new ("org.gtk.notification", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  return status;
}
