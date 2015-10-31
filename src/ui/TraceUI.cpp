//
// TraceUI.h
//
// Handles FLTK integration and other user interface tasks
//
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <FL/fl_ask.h>

#include "TraceUI.h"
#include "../RayTracer.h"

static bool done;

//------------------------------------- Help Functions --------------------------------------------
TraceUI* TraceUI::whoami(Fl_Menu_* o)	// from menu item back to UI itself
{
	return ( (TraceUI*)(o->parent()->user_data()) );
}

//--------------------------------- Callback Functions --------------------------------------------
void TraceUI::cb_load_scene(Fl_Menu_* o, void* v) 
{
	TraceUI* pUI=whoami(o);
	
	char* newfile = fl_file_chooser("Open Scene?", "*.ray", NULL );

	if (newfile != NULL) {
		char buf[256];

		if (pUI->raytracer->loadScene(newfile)) {
			sprintf(buf, "Ray <%s>", newfile);
			done=true;	// terminate the previous rendering
		} else{
			sprintf(buf, "Ray <Not Loaded>");
		}

		pUI->m_mainWindow->label(buf);
	}
}

void TraceUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	TraceUI* pUI=whoami(o);
	
	char* savefile = fl_file_chooser("Save Image?", "*.bmp", "save.bmp" );
	if (savefile != NULL) {
		pUI->m_traceGlWindow->saveImage(savefile);
	}
}

void TraceUI::cb_exit(Fl_Menu_* o, void* v)
{
	TraceUI* pUI=whoami(o);

	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_exit2(Fl_Widget* o, void* v) 
{
	TraceUI* pUI=(TraceUI *)(o->user_data());
	
	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("RayTracer Project, FLTK version for CS 341 Spring 2002. Latest modifications by Jeff Maurer, jmaurer@cs.washington.edu");
}

void TraceUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	TraceUI* pUI=(TraceUI*)(o->user_data());
	
	pUI->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
	int	height = (int)(pUI->m_nSize / pUI->raytracer->aspectRatio() + 0.5);
	pUI->m_traceGlWindow->resizeWindow( pUI->m_nSize, height );
}

void TraceUI::cb_constAttenuationSlides(Fl_Widget* o, void* v) {
	((TraceUI*)(o->user_data()))->m_nConstAttenuation = double(((Fl_Slider *)o)->value());
}

void TraceUI::cb_linearAttenuationSlides(Fl_Widget* o, void* v) {
	((TraceUI*)(o->user_data()))->m_nLinearAttenuation = double(((Fl_Slider *)o)->value());
}

void TraceUI::cb_quadAttenuationSlides(Fl_Widget* o, void* v) {
	((TraceUI*)(o->user_data()))->m_nQuadAttenuation = double(((Fl_Slider *)o)->value());
}

void TraceUI::cb_ambientLightSlides(Fl_Widget* o, void* v) {
	((TraceUI*)(o->user_data()))->m_nAmbientLight = double(((Fl_Slider *)o)->value());
}

void TraceUI::cb_intensitySlides(Fl_Widget* o, void* v) {
	((TraceUI*)(o->user_data()))->m_nIntensity = int(((Fl_Slider *)o)->value());
}

void TraceUI::cb_distanceSlides(Fl_Widget* o, void* v) {
	((TraceUI*)(o->user_data()))->m_nDistance = double(((Fl_Slider *)o)->value());
}

void TraceUI::cb_depthSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nDepth=int( ((Fl_Slider *)o)->value() ) ;
}

void TraceUI::cb_render(Fl_Widget* o, void* v)
{
	char buffer[256];

	TraceUI* pUI=((TraceUI*)(o->user_data()));
	
	if (pUI->raytracer->sceneLoaded()) {
		int width=pUI->getSize();
		int	height = (int)(width / pUI->raytracer->aspectRatio() + 0.5);
		pUI->m_traceGlWindow->resizeWindow( width, height );

		pUI->m_traceGlWindow->show();

		pUI->raytracer->traceSetup(width, height);
		
		// Save the window label
		const char *old_label = pUI->m_traceGlWindow->label();

		// start to render here	
		done=false;
		clock_t prev, now;
		prev=clock();
		
		pUI->m_traceGlWindow->refresh();
		Fl::check();
		Fl::flush();

		for (int y=0; y<height; y++) {
			for (int x=0; x<width; x++) {
				if (done) break;
				
				// current time
				now = clock();

				// check event every 1/2 second
				if (((double)(now-prev)/CLOCKS_PER_SEC)>0.5) {
					prev=now;

					if (Fl::ready()) {
						// refresh
						pUI->m_traceGlWindow->refresh();
						// check event
						Fl::check();

						if (Fl::damage()) {
							Fl::flush();
						}
					}
				}

				pUI->raytracer->tracePixel( x, y );
		
			}
			if (done) break;

			// flush when finish a row
			if (Fl::ready()) {
				// refresh
				pUI->m_traceGlWindow->refresh();

				if (Fl::damage()) {
					Fl::flush();
				}
			}
			// update the window label
			sprintf(buffer, "(%d%%) %s", (int)((double)y / (double)height * 100.0), old_label);
			pUI->m_traceGlWindow->label(buffer);
			
		}
		done=true;
		pUI->m_traceGlWindow->refresh();

		// Restore the window label
		pUI->m_traceGlWindow->label(old_label);		
	}
}

void TraceUI::cb_stop(Fl_Widget* o, void* v)
{
	done=true;
}

void TraceUI::show()
{
	m_mainWindow->show();
}

void TraceUI::setRayTracer(RayTracer *tracer)
{
	raytracer = tracer;
	m_traceGlWindow->setRayTracer(tracer);
}

int TraceUI::getSize()
{
	return m_nSize;
}

int TraceUI::getDepth()
{
	return m_nDepth;
}

double TraceUI::getConstAttenuationVal() {
	return m_nConstAttenuation;
}

double TraceUI::getLinearAttenuationVal() {
	return m_nLinearAttenuation;
}

double TraceUI::getQuadAttenuationVal() {
	return m_nQuadAttenuation;
}

double TraceUI::getAmbientLightValue() {
	return m_nAmbientLight;
}

int TraceUI::getIntensityScale() {
	return m_nIntensity;
}

double TraceUI::getDistanceScale() {
	return m_nDistance;
}

void TraceUI::setConstAttenuationVal(double value){
	m_nConstAttenuation = value;
}
void TraceUI::setLinearAttenuationVal(double value) {
	m_nLinearAttenuation = value;
}
void TraceUI::setQuadAttenuationVal(double value) {
	m_nQuadAttenuation = value;
}

// menu definition
Fl_Menu_Item TraceUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Scene...",	FL_ALT + 'l', (Fl_Callback *)TraceUI::cb_load_scene },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)TraceUI::cb_save_image },
		{ "&Exit",			FL_ALT + 'e', (Fl_Callback *)TraceUI::cb_exit },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)TraceUI::cb_about },
		{ 0 },

	{ 0 }
};

TraceUI::TraceUI() {
	// init.
	m_nDepth = 0;
	m_nSize = 150;
	m_nConstAttenuation = 0.25;
	m_nLinearAttenuation = 0.25;
	m_nQuadAttenuation = 0.50;
	m_nAmbientLight = 0.20;
	m_nIntensity = 1;
	m_nDistance = 1.87;

	m_mainWindow = new Fl_Window(100, 40, 400, 250, "Ray <Not Loaded>");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 320, 25);
		m_menubar->menu(menuitems);

		// install slider depth
		m_depthSlider = new Fl_Value_Slider(10, 30, 180, 20, "Depth");
		m_depthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_depthSlider->type(FL_HOR_NICE_SLIDER);
        m_depthSlider->labelfont(FL_COURIER);
        m_depthSlider->labelsize(12);
		m_depthSlider->minimum(0);
		m_depthSlider->maximum(10);
		m_depthSlider->step(1);
		m_depthSlider->value(m_nDepth);
		m_depthSlider->align(FL_ALIGN_RIGHT);
		m_depthSlider->callback(cb_depthSlides);

		// install slider size
		m_sizeSlider = new Fl_Value_Slider(10, 55, 180, 20, "Size");
		m_sizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_sizeSlider->type(FL_HOR_NICE_SLIDER);
        m_sizeSlider->labelfont(FL_COURIER);
        m_sizeSlider->labelsize(12);
		m_sizeSlider->minimum(64);
		m_sizeSlider->maximum(512);
		m_sizeSlider->step(1);
		m_sizeSlider->value(m_nSize);
		m_sizeSlider->align(FL_ALIGN_RIGHT);
		m_sizeSlider->callback(cb_sizeSlides);

		// install constant attenuation slider
		m_constAttenuationSlider = new Fl_Value_Slider(10, 80, 180, 20, "Attenuation, Constant");
		m_constAttenuationSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_constAttenuationSlider->type(FL_HOR_NICE_SLIDER);
		m_constAttenuationSlider->labelfont(FL_COURIER);
		m_constAttenuationSlider->labelsize(12);
		m_constAttenuationSlider->minimum(0.00);
		m_constAttenuationSlider->maximum(1.00);
		m_constAttenuationSlider->step(0.01);
		m_constAttenuationSlider->value(m_nConstAttenuation);
		m_constAttenuationSlider->align(FL_ALIGN_RIGHT);
		m_constAttenuationSlider->callback(cb_constAttenuationSlides);

		// install linear attenuation slider
		m_constAttenuationSlider = new Fl_Value_Slider(10, 105, 180, 20, "Attenuation, Linear");
		m_constAttenuationSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_constAttenuationSlider->type(FL_HOR_NICE_SLIDER);
		m_constAttenuationSlider->labelfont(FL_COURIER);
		m_constAttenuationSlider->labelsize(12);
		m_constAttenuationSlider->minimum(0.00);
		m_constAttenuationSlider->maximum(1.00);
		m_constAttenuationSlider->step(0.01);
		m_constAttenuationSlider->value(m_nLinearAttenuation);
		m_constAttenuationSlider->align(FL_ALIGN_RIGHT);
		m_constAttenuationSlider->callback(cb_linearAttenuationSlides);

		// install quad attenuation slider
		m_constAttenuationSlider = new Fl_Value_Slider(10, 130, 180, 20, "Attenuation, Quadratic");
		m_constAttenuationSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_constAttenuationSlider->type(FL_HOR_NICE_SLIDER);
		m_constAttenuationSlider->labelfont(FL_COURIER);
		m_constAttenuationSlider->labelsize(12);
		m_constAttenuationSlider->minimum(0.00);
		m_constAttenuationSlider->maximum(1.00);
		m_constAttenuationSlider->step(0.01);
		m_constAttenuationSlider->value(m_nQuadAttenuation);
		m_constAttenuationSlider->align(FL_ALIGN_RIGHT);
		m_constAttenuationSlider->callback(cb_quadAttenuationSlides);

		// install ambient light slider
		m_constAttenuationSlider = new Fl_Value_Slider(10, 155, 180, 20, "Ambient Light");
		m_constAttenuationSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_constAttenuationSlider->type(FL_HOR_NICE_SLIDER);
		m_constAttenuationSlider->labelfont(FL_COURIER);
		m_constAttenuationSlider->labelsize(12);
		m_constAttenuationSlider->minimum(0.00);
		m_constAttenuationSlider->maximum(1.00);
		m_constAttenuationSlider->step(0.01);
		m_constAttenuationSlider->value(m_nAmbientLight);
		m_constAttenuationSlider->align(FL_ALIGN_RIGHT);
		m_constAttenuationSlider->callback(cb_ambientLightSlides);

		// install intensity scale slider
		m_constAttenuationSlider = new Fl_Value_Slider(10, 180, 180, 20, "Intensity Scale");
		m_constAttenuationSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_constAttenuationSlider->type(FL_HOR_NICE_SLIDER);
		m_constAttenuationSlider->labelfont(FL_COURIER);
		m_constAttenuationSlider->labelsize(12);
		m_constAttenuationSlider->minimum(1);
		m_constAttenuationSlider->maximum(10);
		m_constAttenuationSlider->step(1);
		m_constAttenuationSlider->value(m_nIntensity);
		m_constAttenuationSlider->align(FL_ALIGN_RIGHT);
		m_constAttenuationSlider->callback(cb_intensitySlides);

		// install distance scale slider
		m_constAttenuationSlider = new Fl_Value_Slider(10, 205, 180, 20, "Distance Scale (Log10)");
		m_constAttenuationSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_constAttenuationSlider->type(FL_HOR_NICE_SLIDER);
		m_constAttenuationSlider->labelfont(FL_COURIER);
		m_constAttenuationSlider->labelsize(12);
		m_constAttenuationSlider->minimum(-0.99);
		m_constAttenuationSlider->maximum(3.00);
		m_constAttenuationSlider->step(0.01);
		m_constAttenuationSlider->value(m_nDistance);
		m_constAttenuationSlider->align(FL_ALIGN_RIGHT);
		m_constAttenuationSlider->callback(cb_distanceSlides);

		m_renderButton = new Fl_Button(240, 27, 70, 25, "&Render");
		m_renderButton->user_data((void*)(this));
		m_renderButton->callback(cb_render);

		m_stopButton = new Fl_Button(240, 55, 70, 25, "&Stop");
		m_stopButton->user_data((void*)(this));
		m_stopButton->callback(cb_stop);

		m_mainWindow->callback(cb_exit2);
		m_mainWindow->when(FL_HIDE);
    m_mainWindow->end();

	// image view
	m_traceGlWindow = new TraceGLWindow(100, 150, m_nSize, m_nSize, "Rendered Image");
	m_traceGlWindow->end();
	m_traceGlWindow->resizable(m_traceGlWindow);
}