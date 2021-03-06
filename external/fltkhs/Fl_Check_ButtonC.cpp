#include "Fl_Check_ButtonC.h"

#ifdef __cplusplus
EXPORT
{
#endif
    FL_EXPORT_C (int, Fl_Check_Button_handle) (fl_Check_Button self, int event)
    {
        return (static_cast<Fl_Check_Button *> (self))->handle (event);
    }
    FL_EXPORT_C (fl_Group, Fl_Check_Button_parent) (fl_Check_Button b)
    {
        return (fl_Group) (static_cast<Fl_Check_Button *> (b))->parent ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_parent)(fl_Check_Button b,fl_Group grp)
    {
        (static_cast<Fl_Check_Button *> (b))
            ->parent ((static_cast<Fl_Group *> (grp)));
    }
    FL_EXPORT_C (uchar, Fl_Check_Button_type) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->type ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_set_type) (fl_Check_Button b, uchar t)
    {
        (static_cast<Fl_Check_Button *> (b))->type (t);
    }

    FL_EXPORT_C (int, Fl_Check_Button_x) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->x ();
    }
    FL_EXPORT_C (int, Fl_Check_Button_y) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->y ();
    }
    FL_EXPORT_C (int, Fl_Check_Button_w) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->w ();
    }
    FL_EXPORT_C (int, Fl_Check_Button_h) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->h ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_align)(fl_Check_Button check_button, Fl_Align alignment)
    {
        (static_cast<Fl_Check_Button *> (check_button))->align (alignment);
    }
    FL_EXPORT_C (Fl_Align, Fl_Check_Button_align) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->align ();
    }
    FL_EXPORT_C (Fl_Boxtype, Fl_Check_Button_box) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->box ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_box)(fl_Check_Button b,Fl_Boxtype new_box)
    {
        (static_cast<Fl_Check_Button *> (b))
            ->box ((static_cast<Fl_Boxtype> (new_box)));
    }
    FL_EXPORT_C (Fl_Color, Fl_Check_Button_color) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->color ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_color)(fl_Check_Button b,Fl_Color bg)
    {
        (static_cast<Fl_Check_Button *> (b))->color (bg);
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_color_with_bg_sel)(fl_Check_Button b,Fl_Color bg,Fl_Color a)
    {
        (static_cast<Fl_Check_Button *> (b))->color (bg, a);
    }
    FL_EXPORT_C (Fl_Color, Fl_Check_Button_selection_color) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->selection_color ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_selection_color)(fl_Check_Button b,Fl_Color a)
    {
        (static_cast<Fl_Check_Button *> (b))->selection_color (a);
    }
    FL_EXPORT_C (const char *, Fl_Check_Button_label) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->label ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_copy_label)(fl_Check_Button b,const char* new_label)
    {
        (static_cast<Fl_Check_Button *> (b))->copy_label (new_label);
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_label)(fl_Check_Button b,const char* text)
    {
        (static_cast<Fl_Check_Button *> (b))->label (text);
    }
    FL_EXPORT_C (Fl_Labeltype, Fl_Check_Button_labeltype) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->labeltype ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_labeltype)(fl_Check_Button b,Fl_Labeltype a)
    {
        (static_cast<Fl_Check_Button *> (b))->labeltype (a);
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_labelcolor)(fl_Check_Button b,Fl_Color c)
    {
        (static_cast<Fl_Check_Button *> (b))->labelcolor (c);
    }
    FL_EXPORT_C (Fl_Color, Fl_Check_Button_labelcolor) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->labelcolor ();
    }
    FL_EXPORT_C (Fl_Font, Fl_Check_Button_labelfont) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->labelfont ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_labelfont)(fl_Check_Button b,Fl_Font c)
    {
        (static_cast<Fl_Check_Button *> (b))
            ->labelfont ((static_cast<Fl_Font> (c)));
    }
    FL_EXPORT_C (Fl_Fontsize, Fl_Check_Button_labelsize) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->labelsize ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_labelsize)(fl_Check_Button b,Fl_Fontsize pix)
    {
        (static_cast<Fl_Check_Button *> (b))
            ->labelsize ((static_cast<Fl_Fontsize> (pix)));
    }
    FL_EXPORT_C (fl_Image, Fl_Check_Button_image) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->image ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_image)(fl_Check_Button b,fl_Image pix)
    {
        (static_cast<Fl_Check_Button *> (b))
            ->image ((static_cast<Fl_Image *> (pix)));
    }
    FL_EXPORT_C (fl_Image, Fl_Check_Button_deimage) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->deimage ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_deimage)(fl_Check_Button b,fl_Image pix)
    {
        (static_cast<Fl_Check_Button *> (b))
            ->deimage ((static_cast<Fl_Image *> (pix)));
    }
    FL_EXPORT_C (const char *, Fl_Check_Button_tooltip) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->tooltip ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_copy_tooltip)(fl_Check_Button b,const char* text)
    {
        (static_cast<Fl_Check_Button *> (b))->copy_tooltip (text);
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_tooltip)(fl_Check_Button b,const char* text)
    {
        (static_cast<Fl_Check_Button *> (b))->tooltip (text);
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_callback_with_user_data)(fl_Check_Button b,fl_Callback* cb,void* p)
    {
        Fl_Check_Button * castedButton = (static_cast<Fl_Check_Button *> (b));
        new C_to_Fl_Callback (castedButton, cb, p);
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_callback)(fl_Check_Button b,fl_Callback* cb)
    {
        Fl_Check_Button * castedButton = (static_cast<Fl_Check_Button *> (b));
        new C_to_Fl_Callback (castedButton, cb);
    }
    FL_EXPORT_C (void *, Fl_Check_Button_user_data) (fl_Check_Button b)
    {
        C_to_Fl_Callback * stored_cb = (static_cast<C_to_Fl_Callback *> (
            (static_cast<Fl_Check_Button *> (b))->user_data ()));
        if (stored_cb)
        {
            return stored_cb->get_user_data ();
        }
        else
        {
            return (static_cast<Fl_Check_Button *> (b))->user_data ();
        }
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_user_data)(fl_Check_Button b,void* v)
    {
        C_to_Fl_Callback * stored_cb = (static_cast<C_to_Fl_Callback *> (
            (static_cast<Fl_Check_Button *> (b))->user_data ()));
        if (stored_cb)
        {
            stored_cb->set_user_data (v);
            (static_cast<Fl_Check_Button *> (b))->user_data (stored_cb);
        }
        else
        {
            (static_cast<Fl_Check_Button *> (b))->user_data (v);
        }
    }
    FL_EXPORT_C (long, Fl_Check_Button_argument) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->argument ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_set_argument) (fl_Check_Button b, long v)
    {
        (static_cast<Fl_Check_Button *> (b))->argument (v);
    }
    FL_EXPORT_C (Fl_When, Fl_Check_Button_when) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->when ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_set_when) (fl_Check_Button b, uchar i)
    {
        (static_cast<Fl_Check_Button *> (b))->when (i);
    }
    FL_EXPORT_C (unsigned int, Fl_Check_Button_visible) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->visible ();
    }
    FL_EXPORT_C (int, Fl_Check_Button_visible_r) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->visible_r ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_set_visible) (fl_Check_Button b)
    {
        (static_cast<Fl_Check_Button *> (b))->visible ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_clear_visible) (fl_Check_Button b)
    {
        (static_cast<Fl_Check_Button *> (b))->clear_visible ();
    }
    FL_EXPORT_C (unsigned int, Fl_Check_Button_active) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->active ();
    }
    FL_EXPORT_C (int, Fl_Check_Button_active_r) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->active_r ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_activate) (fl_Check_Button b)
    {
        (static_cast<Fl_Check_Button *> (b))->activate ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_deactivate) (fl_Check_Button b)
    {
        (static_cast<Fl_Check_Button *> (b))->deactivate ();
    }
    FL_EXPORT_C (unsigned int, Fl_Check_Button_output) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->output ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_set_output) (fl_Check_Button b)
    {
        (static_cast<Fl_Check_Button *> (b))->output ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_clear_output) (fl_Check_Button b)
    {
        (static_cast<Fl_Check_Button *> (b))->clear_output ();
    }
    FL_EXPORT_C (unsigned int, Fl_Check_Button_takesevents) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->takesevents ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_set_changed) (fl_Check_Button b)
    {
        (static_cast<Fl_Check_Button *> (b))->changed ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_clear_changed) (fl_Check_Button b)
    {
        (static_cast<Fl_Check_Button *> (b))->clear_changed ();
    }
    FL_EXPORT_C (int, Fl_Check_Button_take_focus) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->take_focus ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_set_visible_focus) (fl_Check_Button b)
    {
        (static_cast<Fl_Check_Button *> (b))->set_visible_focus ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_clear_visible_focus) (fl_Check_Button b)
    {
        (static_cast<Fl_Check_Button *> (b))->clear_visible_focus ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_modify_visible_focus)(fl_Check_Button b,int v)
    {
        (static_cast<Fl_Check_Button *> (b))->visible_focus (v);
    }
    FL_EXPORT_C(unsigned int,Fl_Check_Button_visible_focus)(fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->visible_focus ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_do_callback) (fl_Check_Button b)
    {
        (static_cast<Fl_Check_Button *> (b))->do_callback ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_do_callback_with_widget_and_user_data)(fl_Check_Button b,fl_Widget w,long arg)
    {
        (static_cast<Fl_Check_Button *> (b))
            ->do_callback ((static_cast<Fl_Widget *> (w)), arg);
    }
    FL_EXPORT_C(void,Fl_Check_Button_do_callback_with_widget_and_default_user_data)(fl_Check_Button b,fl_Widget w)
    {
        (static_cast<Fl_Check_Button *> (b))
            ->do_callback ((static_cast<Fl_Widget *> (w)));
    }
    FL_EXPORT_C (int, Fl_Check_Button_contains) (fl_Check_Button b, fl_Widget w)
    {
        return (static_cast<Fl_Check_Button *> (b))
            ->contains ((static_cast<Fl_Widget *> (w)));
    }
    FL_EXPORT_C (int, Fl_Check_Button_inside) (fl_Check_Button b, fl_Widget w)
    {
        return (static_cast<Fl_Check_Button *> (b))
            ->inside ((static_cast<Fl_Widget *> (w)));
    }
    FL_EXPORT_C (void, Fl_Check_Button_redraw) (fl_Check_Button b)
    {
        (static_cast<Fl_Check_Button *> (b))->redraw ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_redraw_label) (fl_Check_Button b)
    {
        (static_cast<Fl_Check_Button *> (b))->redraw_label ();
    }
    FL_EXPORT_C (uchar, Fl_Check_Button_damage) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->damage ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_clear_damage_with_bitmask)(fl_Check_Button b,uchar c)
    {
        (static_cast<Fl_Check_Button *> (b))->clear_damage (c);
    }
    FL_EXPORT_C (void, Fl_Check_Button_clear_damage) (fl_Check_Button b)
    {
        (static_cast<Fl_Check_Button *> (b))->clear_damage ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_damage_with_text)(fl_Check_Button b,uchar c)
    {
        (static_cast<Fl_Check_Button *> (b))->damage (c);
    }
    FL_EXPORT_C(void,Fl_Check_Button_damage_inside_widget)(fl_Check_Button b,uchar c,int x,int y,int w,int h)
    {
        (static_cast<Fl_Check_Button *> (b))->damage (c, x, y, w, h);
    }
    FL_EXPORT_C(void,Fl_Check_Button_draw_label_with_xywh_alignment)(fl_Check_Button b,int x,int y,int w,int h,Fl_Align alignment)
    {
        (static_cast<Fl_Check_Button *> (b))
            ->draw_label (x, y, w, h, alignment);
    }
    FL_EXPORT_C(void,Fl_Check_Button_measure_label)(fl_Check_Button b,int* ww,int* hh)
    {
        (static_cast<Fl_Check_Button *> (b))->measure_label (*ww, *hh);
    }

    FL_EXPORT_C(fl_Check_Button, Fl_Check_Button_New_WithLabel)(int x, int y, int w, int h, const char* label)
    {
        Fl_Check_Button * button = new Fl_Check_Button (x, y, w, h, label);
        return (static_cast<fl_Check_Button> (button));
    }
    FL_EXPORT_C(fl_Check_Button, Fl_Check_Button_New)(int x, int y, int w, int h)
    {
        Fl_Check_Button * button = new Fl_Check_Button (x, y, w, h, 0);
        return (fl_Check_Button)button;
    }
    FL_EXPORT_C (void, Fl_Check_Button_Destroy) (fl_Check_Button button)
    {
        delete (static_cast<Fl_Check_Button *> (button));
    }
    FL_EXPORT_C (int, Fl_Check_Button_set_value) (fl_Check_Button b, int v)
    {
        return (static_cast<Fl_Check_Button *> (b))->value (v);
    }
    FL_EXPORT_C (char, Fl_Check_Button_value) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->value ();
    }
    FL_EXPORT_C (int, Fl_Check_Button_clear) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->clear ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_setonly) (fl_Check_Button b)
    {
        (static_cast<Fl_Check_Button *> (b))->setonly ();
    }
    FL_EXPORT_C (int, Fl_Check_Button_get_shortcut) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->shortcut ();
    }
    FL_EXPORT_C (void, Fl_Check_Button_set_shortcut) (fl_Check_Button b, int s)
    {
        (static_cast<Fl_Check_Button *> (b))->shortcut (s);
    }
    FL_EXPORT_C (Fl_Boxtype, Fl_Check_Button_down_box) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->down_box ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_down_box)(fl_Check_Button b,Fl_Boxtype boxtype)
    {
        (static_cast<Fl_Check_Button *> (b))->down_box (boxtype);
    }
    FL_EXPORT_C (Fl_Color, Fl_Check_Button_down_color) (fl_Check_Button b)
    {
        return (static_cast<Fl_Check_Button *> (b))->down_color ();
    }
    FL_EXPORT_C(void,Fl_Check_Button_set_down_color)(fl_Check_Button b,Fl_Color c)
    {
        (static_cast<Fl_Check_Button *> (b))->down_color (c);
    }
#ifdef __cplusplus
}
#endif
