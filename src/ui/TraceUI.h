//
// rayUI.h
//
// The header file for the UI part
//

#ifndef __rayUI_h__
#define __rayUI_h__

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>

#include <FL/fl_file_chooser.H>		// FLTK file chooser

#include "TraceGLWindow.h"

class TraceUI {
public:
	TraceUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;

	Fl_Slider*			m_sizeSlider;
	Fl_Slider*			m_depthSlider;
	Fl_Slider*			m_constAttenuationSlider;
	Fl_Slider*			m_linearAttenuationSlider;
	Fl_Slider*			m_quadAttenuationSlider;
	Fl_Slider*			m_ambientAttenuationSlider;
	Fl_Slider*			m_intensitySlider;
	Fl_Slider*			m_distanceSlider;

	Fl_Button*			m_renderButton;
	Fl_Button*			m_stopButton;

	TraceGLWindow*		m_traceGlWindow;

	// member functions
	void show();

	void		setRayTracer(RayTracer *tracer);

	int			getSize();
	int			getDepth();
	double		getConstAttenuationVal();
	double		getLinearAttenuationVal();
	double		getQuadAttenuationVal();
	double		getAmbientLightValue();
	int			getIntensityScale();
	double		getDistanceScale();

	void		setConstAttenuationVal(double value);
	void		setLinearAttenuationVal(double value);
	void		setQuadAttenuationVal(double value);

private:
	RayTracer*	raytracer;

	int			m_nSize;
	int			m_nDepth;
	double		m_nConstAttenuation;
	double		m_nLinearAttenuation;
	double		m_nQuadAttenuation;
	double	    m_nAmbientLight;
	int			m_nIntensity;
	double	    m_nDistance;

// static class members
	static Fl_Menu_Item menuitems[];

	static TraceUI* whoami(Fl_Menu_* o);

	static void cb_load_scene(Fl_Menu_* o, void* v);
	static void cb_save_image(Fl_Menu_* o, void* v);
	static void cb_exit(Fl_Menu_* o, void* v);
	static void cb_about(Fl_Menu_* o, void* v);

	static void cb_exit2(Fl_Widget* o, void* v);

	static void cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_depthSlides(Fl_Widget* o, void* v);
	static void cb_constAttenuationSlides(Fl_Widget* o, void* v);
	static void cb_linearAttenuationSlides(Fl_Widget* o, void* v);
	static void cb_quadAttenuationSlides(Fl_Widget* o, void* v);
	static void cb_ambientLightSlides(Fl_Widget* o, void* v);
	static void cb_intensitySlides(Fl_Widget* o, void* v);
	static void cb_distanceSlides(Fl_Widget* o, void* v);

	static void cb_render(Fl_Widget* o, void* v);
	static void cb_stop(Fl_Widget* o, void* v);
};

#endif
