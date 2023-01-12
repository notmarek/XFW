#include "../bad_usb_app_i.h"
#include "../../../settings/desktop_settings/desktop_settings_app.h"

typedef enum {
    BadUsbCustomEventErrorBack,
} BadUsbCustomEvent;

static void
    bad_usb_scene_error_event_callback(GuiButtonType result, InputType type, void* context) {
    furi_assert(context);
    BadUsbApp* app = context;

    if((result == GuiButtonTypeLeft) && (type == InputTypeShort)) {
        view_dispatcher_send_custom_event(app->view_dispatcher, BadUsbCustomEventErrorBack);
    }
}

void bad_usb_scene_error_on_enter(void* context) {
    BadUsbApp* app = context;
    DesktopSettings* settings = malloc(sizeof(DesktopSettings));
    DESKTOP_SETTINGS_LOAD(settings);

    if(app->error == BadUsbAppErrorNoFiles) {
        widget_add_icon_element(app->widget, 0, 0, &I_SDQuestion_35x43);
        widget_add_string_multiline_element(
            app->widget,
            81,
            4,
            AlignCenter,
            AlignTop,
            FontSecondary,
            "No SD card or\napp data found.\nThis app will not\nwork without\nrequired files.");
        widget_add_button_element(
            app->widget, GuiButtonTypeLeft, "Back", bad_usb_scene_error_event_callback, app);
    } else if(app->error == BadUsbAppErrorCloseRpc) {
        widget_add_icon_element(app->widget, 78, 0, &I_ActiveConnection_50x64);
        if(settings->sfw_mode) {
            widget_add_string_multiline_element(
                app->widget, 3, 2, AlignLeft, AlignTop, FontPrimary, "Connection\nis active!");
            widget_add_string_multiline_element(
                app->widget,
                3,
                30,
                AlignLeft,
                AlignTop,
                FontSecondary,
                "Disconnect from\nPC or phone to\nuse this function.");
        } else {
            widget_add_string_multiline_element(
                app->widget, 3, 2, AlignLeft, AlignTop, FontPrimary, "I am not\na whore!");
            widget_add_string_multiline_element(
                app->widget,
                3,
                30,
                AlignLeft,
                AlignTop,
                FontSecondary,
                "Pull out from\nPC or phone to\nuse me like this.");
        }
    }

    view_dispatcher_switch_to_view(app->view_dispatcher, BadUsbAppViewError);
    free(settings);
}

bool bad_usb_scene_error_on_event(void* context, SceneManagerEvent event) {
    BadUsbApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == BadUsbCustomEventErrorBack) {
            view_dispatcher_stop(app->view_dispatcher);
            consumed = true;
        }
    }
    return consumed;
}

void bad_usb_scene_error_on_exit(void* context) {
    BadUsbApp* app = context;
    widget_reset(app->widget);
}
