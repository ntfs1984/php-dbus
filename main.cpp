#include <phpcpp.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <glib/gprintf.h>
#include <gio/gio.h>
class Dbus : public Php::Base {
private:
GDBusConnection *connection;
bool verbose = false;
public:
    void __construct();
    Php::Value Open(Php::Parameters &params);
    Php::Value Verbose(Php::Parameters &params);
    Php::Value CallMethod(Php::Parameters &params);
    Php::Value ListNames(Php::Parameters &params);
    Php::Value GetProperty(Php::Parameters &params);
    Php::Value GetAll(Php::Parameters &params);
    Php::Value ListServices(Php::Parameters &params);
    void Close(Php::Parameters &params);
};

    void Dbus::__construct()    {

} 
// -------------------------------------------------------------------------------------------------                                    
/* Open method creates connection to d-bus
 * For now we can have only one connection per script. May be I will expand it later.
 * Example usage in PHP:
 * $dbus = new Dbus;
 * $dbus->Open("DBUS_BUS_SESSION");
 * That's all.
*/ 
    Php::Value Dbus::Open(Php::Parameters &params) {
    std::string type = params[0];
    if (type=="G_BUS_TYPE_SESSION") {connection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, NULL);return connection;}
    if (type=="G_BUS_TYPE_SYSTEM") {connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);return connection;}
    if (verbose) {g_printerr("\n [PHP-DBUS error] Unknown G_DBUS_TYPE.\n Correct usage is: $Dbus->Open('G_BUS_TYPE_SESSION') or $Dbus->Open('G_BUS_TYPE_SYSTEM')\n");}
    else {g_printerr("Error: unknown G_DBUS_TYPE.\n");}
    
    return false;
}
                                                   
    void Dbus::Close(Php::Parameters &params) {
    g_object_unref(connection);
                                                   }                                              

  Php::Value Dbus::Verbose(Php::Parameters &params) {
  bool v = params[0];
  if (v==true) {verbose = true;}
  if (v==false) {verbose = false;}
  return true;                         
} 

  
  
/*
 * CallMethod - calling some dbus method with some parameters
 * Currently supported parameters: single string, double string, single int, double int.
 * 
 */
   
  Php::Value Dbus::CallMethod(Php::Parameters &params) {
   const char *bus = params[0];
   const char *object = params[1];
   const char *interface = params[2];
   const char *method = params[3];
   const char *par_type = params[4];
   const char *par_value = params[5];
   GDBusProxy *proxy;
   GVariant *result;
   char *y;
   proxy = g_dbus_proxy_new_sync(connection,
                                  G_DBUS_PROXY_FLAGS_NONE,
                                  NULL, /* GDBusInterfaceInfo */
                                  bus,
                                  object,
                                  interface,
                                  NULL, /* GCancellable */
                                  NULL); /* GError ** */
    GError *error = NULL;
    
    result = g_dbus_proxy_call_sync(proxy,
                                 method,
                                 g_variant_new (par_type,par_value), /* GVariant *parameters */
                                 G_DBUS_CALL_FLAGS_NONE,
                                 -1, /* gint timeout_msec */
                                 NULL, /* GCancellable */
                                 &error); /* GError ** */
    if (error != NULL) {
    if (verbose) {g_printerr("\n [PHP-DBUS error] %s \n This might be happen because method CallMethod not found on requested path, or path/interface doesn't exist at all, try to use tool d-feet to determine correct path. \n Example usage: $dbus->CallMethod(\":1.785\",\"/StatusNotifierItem\",\"org.kde.StatusNotifierItem\",\"Activate\",\"(ii)\",\"0,0\");\n", error->message);}
    else {g_printerr("Error: %s \n", error->message);}
    g_error_free(error);
    return false;
}

    y = (char*)g_variant_print(result, false);
     /* Free resources */
    g_variant_unref(result);
    g_object_unref(proxy);
    return y;
                           
}               
// -------------------------------------------------------------------------------------------------                                    
/* GetAll method usually returns all methods using by interface of path
 * This function returns STRING, not array, because PHP have more powerful tools to parse strings and arrays, than C.
 * Example usage in PHP:
 * $dbus = new Dbus;
 * $dbus->Open("DBUS_BUS_SESSION");
 * $x = $dbus->GetAll(":1.646","/StatusNotifierItem","org.freedesktop.DBus.Properties");
 * echo $x;
 * That's all.
*/ 
 Php::Value Dbus::GetAll(Php::Parameters &params) {
   const char *bus = params[0];
   const char *object = params[1];
   const char *interface = params[2];
   GDBusProxy *proxy;
   GVariant *result;
   char *y;
   proxy = g_dbus_proxy_new_sync(connection,
                                  G_DBUS_PROXY_FLAGS_NONE,
                                  NULL, /* GDBusInterfaceInfo */
                                  bus,
                                  object,
                                  interface,
                                  NULL, /* GCancellable */
                                  NULL); /* GError ** */
    GError *error = NULL;
    result = g_dbus_proxy_call_sync(proxy,
                                 "GetAll",
                                 g_variant_new ("(s)",""), /* GVariant *parameters */
                                 G_DBUS_CALL_FLAGS_NONE,
                                 -1, /* gint timeout_msec */
                                 NULL, /* GCancellable */
                                 &error); /* GError ** */
    if (error != NULL) {
    if (verbose) {g_printerr("[PHP-DBUS error] %s \n This might be happen because method GetAll not found on requested path, or path/interface doesn't exist at all, try to use tool d-feet to determine correct path \n", error->message);}
    else {g_printerr("Error: %s \n", error->message);}
    g_error_free(error);
    return false;
}

    y = (char*)g_variant_print(result, false);
     /* Free resources */
    g_variant_unref(result);
    g_object_unref(proxy);
    return y;
} 
// -------------------------------------------------------------------------------------------------
/* ListNames method just returning all buses (services) registered in system\session
 * It returns simple digit-indexed array of strings.
 * Example usage in PHP:
 * $dbus = new Dbus;
 * $dbus->Open("DBUS_BUS_SESSION");
 * $x = $dbus->ListNames();
 * print_r($x);
*/  
  Php::Value Dbus::ListNames(Php::Parameters &params) {
    GDBusProxy *proxy;
    GVariant *result;
    char **names;
    Php::Value array;
    guint i;
    proxy = g_dbus_proxy_new_sync(connection,
                                  G_DBUS_PROXY_FLAGS_NONE,
                                  NULL, /* GDBusInterfaceInfo */
                                  "org.freedesktop.DBus",
                                  "/org/freedesktop/DBus",
                                  "org.freedesktop.DBus",
                                  NULL, /* GCancellable */
                                  NULL); /* GError ** */

    /* Call the "ListNames" method */
    GError *error = NULL;
    result = g_dbus_proxy_call_sync(proxy,
                                    "ListNames",
                                    NULL, /* GVariant *parameters */
                                    G_DBUS_CALL_FLAGS_NONE,
                                    -1, /* gint timeout_msec */
                                    NULL, /* GCancellable */
                                    NULL); /* GError ** */
    if (error != NULL) {
    g_printerr("Error: %s\n", error->message);
    g_error_free(error);
    return "Error";
}
    // g_variant_get_type_string(result);
    /* Extract the names from the result */
    GVariant *reply_child = g_variant_get_child_value(result, 0);
    names = (char**)g_variant_get_strv(reply_child, NULL);
    /* Print the names */
   for (i = 0; names[i] != NULL; i++) {
	array[i] = names[i];   
    }
    /* Free resources */
    g_variant_unref(result);
    g_object_unref(proxy);
    return array;
}  
 
 
extern "C" {
    PHPCPP_EXPORT void *get_module() 
    {
        static Php::Extension extension("php-dbus", "1.0");
        Php::Class<Dbus> dbus("Dbus");
        dbus.method<&Dbus::__construct>("__construct");
        dbus.method<&Dbus::Open>("Open");
        dbus.method<&Dbus::Close>("Close");
        dbus.method<&Dbus::Verbose>("Verbose");
        dbus.method<&Dbus::CallMethod>("CallMethod");
        dbus.method<&Dbus::GetAll>("GetAll");
        dbus.method<&Dbus::ListNames>("ListNames", { 
        Php::ByVal("change", Php::Type::Array, false) 
        });
        extension.add(std::move(dbus));
     
        return extension;
    }
}
