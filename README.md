# Trace
Trace is a program that constructs recursively ray-traced images of fairly simple scenes. 

After running the sample solution, you should build the skeleton code and see how it compares. You will probably notice that there is a significant difference in the quality of images rendered with the two versions. This suggests what parts of the raytracer have been written and what parts are left undone.

If you compare the outputs of the skeleton and solution, you will see that most of the basic geometry-handling code is done. The skeleton code is able to cast rays into an image and draw color on the screen, resulting in some flat-shaded polygonal shapes.  The skeleton code is doing ray-casting and nothing more.  Furthermore, the box primitive will not appear. While all the code to cast a ray exists, not all of the object intersections code is there.  You need to implement the box intersections and expand ray-casting into ray-tracing by adding support for reflected and refracted rays. You also must implement the Phong specular-reflection model and include support for transparent objects.

Specifically, each group must implement recursive ray tracing as described in class. This entails making the following extensions to the program:

|   Extension   | Foley, et al. | Alan Watt  | Glassner |
| ------------- |:-------------:| ----------:|  -------:|
| Box Intersection     | - | pp 21-22| - |
| Phong specular-reflection model| 16.1 and 16.2.5     |  6.2-6.3 | - |
| Contribution from: multiple light sources, distance attenuation, and these additional types of lights: Directional lights Point lights | 16.1 (particularly in 16.1.5 and 16.1.6)     |  - | - |
| Shadow Attenuation| 16.12 and 16.4    |  9 | - |
| Reflection| 16.12   |  1.4 | 4.2.1 |
| Refraction| 16.12   |  1.4 | 4.2.3 - 4.2.7 [WARNING](http://courses.cs.washington.edu/courses/cse457/01au/projects/trace/wattwarning.html)|

#### Getting Started

The Trace project is a very large collection of files and object-oriented C++ code. Fortunately, you only need to work directly with a small subset of it. However, you will probably want to spend a bit of time getting familiar with the layout and class hierarchy at first, so that when you code you know what classes and methods are available for your use.

The starting point for where ray tracing begins, and where you will be needing to add a lot of functionality, is in the RayTracer.cpp file. This is a good file to start studying and exploring what methods get called and what they do.

You will also need to modify the read.cpp for supporting the complete file format(those marked in red) as described in format.html and you might need to add more features to the .ray file format if you need to implement the extra extensions.

#### Bells and Whistles

In many cases, the material in the Foley, et al. book goes into more depth on a particular subject than the Alan Watt text. In addition, Glassner's book on ray tracing is a very comprehensive exposition of a whole bunch of ways ray tracing can be expanded or optimized (and it's really well written). If you're planning on implementing any of these bells and whistles, you are encouraged to read the relevant sections in these books as well.

Remember that you'll need to establish to our satisfaction that you've implemented the extension! You should have test cases that clearly demonstrate the effect of the code you've added to the ray tracer. Sometimes different extensions can interact, making it hard to tell how each contributed to the final image, so it's also helpful (that is, necessary!) to add controls to selectively enable and disable your extensions.

1. [ ] ![whistle](http://i.imgur.com/VYSixYv.gif) Implement an adaptive termination criterion for tracing rays, based on ray contribution.  Control the adaptation  threshold with a slider.

2. [ ] ![whistle](http://i.imgur.com/VYSixYv.gif) Implement antialiasing by supersampling and averaging down.  You must provide a slider that indicates the number of sub-pixel samples being used (e.g., something in the range 1-5 indicating 1x1, 2x2, ... 5x5 supersampling). This whistle upgrades to a bell if you implement the first stage of distribution ray tracing by jittering the sub-pixel samples.  The noise introduced by jittering should be evident when casting 1 ray per pixel.

3. [ ] ![bell](http://i.imgur.com/HJ7cCdM.gif) Implement spot lights.

4. [ ] ![bell](http://i.imgur.com/HJ7cCdM.gif) Add a menu option that lets you specify a background image to replace the environment's ambient color during the rendering.  That is, any ray that goes off into infinity behind the scene should return a color from the loaded image, instead of just black.  The background should appear as the backplane of the rendered image with suitable reflections and refractions to it.

5. [ ] ![bell](http://i.imgur.com/HJ7cCdM.gif) Find a good way to accelerate shadow attenuation.  Do you need to check against every object when casting the shadow ray?  This one is hard to demonstrate directly, so be prepared to explain in detail how you pulled it off.

6. [ ] ![bell](http://i.imgur.com/HJ7cCdM.gif) Deal with overlapping objects intelligently.  While the skeleton code handles materials with arbitrary indices of refraction, it assumes that objects don't intersect one another. It breaks down when objects intersect or are wholly contained inside other objects. Add support to the refraction code for detecting this and handling it in a more realistic fashion.  Note, however, that in the real world, objects can't coexist in the same place at the same time. You will have to make assumptions as to how to choose the index of refraction in the overlapping space.  Make those assumptions clear when demonstrating the results.

7. [ ] ![bell+whistle](http://i.imgur.com/BxHJUfG.gif) Add texture mapping support to the program. The most basic kind of texture mapping is to apply the map to the diffuse color of a surface. But many other parameters can be mapped. Reflected color can be mapped to create the sense of a surrounding environment. Transparency can be mapped to create holes in objects. Additional (variable) extra credit will be given for such additional mappings.  You will need to modify the parser for handling texture mapping inputs the types of mapping mentioned above.  Additional credit will be awarded for quality implementation of texture mapping on general trimeshes.

8. [ ] ![bell+whistle](http://i.imgur.com/BxHJUfG.gif) Implement antialiasing by adaptive supersampling, as described in Foley, et al., 15.10.4.  For full credit, you must show some sort of visualization of the sampling pattern that results.  For example, you could create another image where each pixel is given an intensity proportional to the number of rays used to calculate the color of the corresponding pixel in the ray traced image.  Implementing this bell/whistle is a big win -- nice antialiasing at low cost.

9. [ ] ![bell+whistle](http://i.imgur.com/BxHJUfG.gif) Implement more versatile lighting controls, such as the Warn model described in Foley 16.1.5. This allows you to do things like control the shape of the projected light.

10. [ ] ![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif) Implement bump mapping (Watt 8.4; Foley, et al. 16.3.3).

11. [ ] ![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif) Implement solid textures or some other form of procedural texture mapping, as described in Foley, et al., 20.1.2 and 20.8.3. Solid textures are a way to easily generate a semi-random texture like wood grain or marble.

12. [ ] ![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif) Add some new types of geometry to the ray tracer. Consider implementing torii or general quadrics. Many other objects are possible here.

13. [ ] ![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif) Add support for height-fields. Click here for a discussion on what they are and how they can be generated.  

14. [ ] ![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif) for first, 

15. [ ] ![bell+whistle](http://i.imgur.com/BxHJUfG.gif) for each additional
Implement distribution ray tracing to produce one or more or the following effects: depth of field, soft shadows, motion blur, or glossy reflection (See Watt 10.6, Glassner, chapter 5, or Foley, et al., 16.12.4).

16. [ ] ![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif) Add some higher-level geometry to the ray tracer, such as surfaces of revolution, extrusions, metaballs or blend surfaces.  You may have implemented one or more of these as a polygonal object in the modeler project.  For the Raytracer, be sure you are actually raytracing the surface as a mathematical construct, not just creating a polygonal representation of the object and tracing that.  Yes, this requires lots of complicated math, but the final results are definitely worth it (see Transparent Metaballs).  A really good tutorial on raytracing metaballs.

17. [ ] ![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif) Implement ray-intersection optimization by either implement the BSP Tree implemented or by implementing a different optimization method, such as hierarchical bounding volumes (See Glassner 6.4 and 6.5, Foley, et al., 15.10.2).

18. [ ] ![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif) Implement a more realistic shading model. Credit will vary depending on the sophistication of the model. A simple model factors in the Fresnel term to compute the amount of light reflected and transmitted at a perfect dielectric (e.g., glass). A more complex model incorporates the notion of a microfacet distribution to broaden the specular highlight. Accounting for the color dependence in the Fresnel term permits a more metallic appearance. Even better, include anisotropic reflections for a plane with parallel grains or a sphere with grains that follow the lines of latitude or longitude. Sources: Watt, Chapter 7, Foley et al, Section 16.7; Glassner, Chapter 4, Section 4; Ward's SIGGRAPH '92 paper; Schlick's Eurographics Rendering Workshop '93 paper.

This all sounds kind of complex, and the physics behind it is. But the coding doesn't have to be. It can be worthwhile to look up one of these alternate models, since they do a much better job at surface shading.  Be sure to demo the results in a way that makes the value added clear.

Theoretically, you could also invent new shading models. For instance, you could implement a less realistic model! Could you implement a shading model that produces something that looks like cel animation? Variable extra credit will be given for these "alternate" shading models. 

19. [ ] ![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif) Implement CSG, constructive solid geometry. This extension allows you to create very interesting models. See page 108 of Glassner for some implementation suggestions.

20. [ ] ![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif) Add a particle systems simulation and renderer (Foley 20.5, Watt 17.7).

21. [ ] ![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif)![bell](http://i.imgur.com/HJ7cCdM.gif) Implement caustics.  Caustics are variations in light intensity caused by refractive focusing--everything from simple magnifying-glass points to the shifting patterns on the bottom of a swimming pool.  An introduction, and a paper discussing a ray-trace project that included caustics.

There are innumerable ways to extend a ray tracer. Think about all the visual phenomena in the real world. The look and shape of cloth. The texture of hair. The look of frost on a window. Dappled sunlight seen through the leaves of a tree. Fire. Rain. The look of things underwater. Prisms. Do you have an idea of how to simulate this phenomenon? Better yet, how can you fake it but get something that looks just as good? You are encouraged to dream up other features you'd like to add to the base ray tracer. Obviously, any such extensions will receive variable extra credit depending on merit (that is, coolness!). Feel free to discuss ideas with the course staff before (and while) proceeding!