#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"

#include "shapes/sphere.h"
#include "shapes/elipsoide.h"
#include "shapes/cylinder.h"
#include "shapes/cone.h"
#include "shapes/cube.h"


#include "cameras/ortographic.h"
#include "cameras/perspective.h"

#include "shaders/intersectionshader.h"
#include "shaders/depthshader.h"
#include "shaders/directshader.h"
#include "shaders/globalshader.h"
#include "shaders/normalshader.h"
#include "shapes/infiniteplane.h"
#include "materials/mirror.h"
#include "materials/transmissive.h"
#include "materials/phong.h"


void newScene(Camera* &cam, Film* &film,
	std::vector<Shape*>* &objectsList,
	std::vector<PointLightSource>* &lightSourceList) {
	
	/* **************************** */
	/* Declare and place the camera */
	/* **************************** */
	Matrix4x4 cameraToWorld = Matrix4x4::translate(Vector3D(0, 0, -3));
	double fovDegrees = 60;
	double fovRadians = Utils::degreesToRadians(fovDegrees);
	cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

	/* ********* */
	/* Materials */
	/* ********* */
	Material *redDiffuse = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0, 0, 0), 100);
	Material *greenDiffuse = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0, 0, 0), 100);
	Material *greyDiffuse = new Phong(Vector3D(0.8, 0.8, 0.8), Vector3D(0,0,0), 50);
	Material *green = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0,0,0), 100);
	Material *blueDiffuse = new Phong(Vector3D(0.3, 0.2, 0.7), Vector3D(0, 0, 0), 100);
	Material *transmissive = new Transmissive(1.2, Vector3D(1));
	Material *mirror = new Mirror(Vector3D(1, 0.9, 0.85));
	Material *red_100 = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0.7, 0.7, 0.2), 100);
	Material *orange = new Phong(Vector3D(1, 127./255, 80./ 255), Vector3D(1, 127./255, 80./ 255), 70);
	Material *groc = new Phong(Vector3D(1, 1, 0.2), Vector3D(1, 1, 0.2), 100);
	Material *brown = new Phong(Vector3D(205./255, 133./255, 63./255),Vector3D(205. / 255, 133. / 255, 63. / 255),30);
	Material *bluesky = new Phong(Vector3D(135. / 255, 206. / 255, 235. / 255), Vector3D(135. / 255, 206. / 255, 235. / 255), 100);
	Material *pink = new Phong(Vector3D(219./255, 112./255, 147.255), Vector3D(219. / 255, 112. / 255, 147.255), 100);

	/* ******* */
	/* Objects */
	/* ******* */
	objectsList = new std::vector<Shape*>;
	double offset = 3.0;
	Matrix4x4 idTransform;
	// Construct the Cornell Box
	Shape *leftPlan = new InfinitePlane(Vector3D(-6, 0, 0), Vector3D(1, 0, 0), greyDiffuse);
	Shape *rightPlan = new InfinitePlane(Vector3D(6, 0, 0), Vector3D(-1, 0, 0), greyDiffuse);
	Shape *topPlan = new InfinitePlane(Vector3D(0, 4.5, 0), Vector3D(0, -1, 0), bluesky);
	Shape *bottomPlan = new InfinitePlane(Vector3D(0, -offset, 0), Vector3D(0, 1, 0), green);
	Shape *midPlan = new InfinitePlane(Vector3D(0, 0, 1), Vector3D(0, 0, -1), transmissive);
	Shape *backPlan = new InfinitePlane(Vector3D(0, 0, 10), Vector3D(0, 0, -1), bluesky);
	objectsList->push_back(leftPlan);
	objectsList->push_back(rightPlan);
	objectsList->push_back(topPlan);
	objectsList->push_back(bottomPlan);
	objectsList->push_back(backPlan);
	//objectsList->push_back(midPlan);

	//----------------
	/*Sphere inside a transmissive Cylinder*/
	Matrix4x4 cylinderTransform;
	cylinderTransform = cylinderTransform.translate(Vector3D(1.1,-0.2,1.5));
	Shape *c1 = new Cylinder(0.4, 0.25, cylinderTransform, transmissive);
	Matrix4x4 sphereTransform1;
	sphereTransform1 = Matrix4x4::translate(Vector3D(1.1, -0.2, 1.5));
	Shape *s1 = new Sphere(0.2, sphereTransform1, pink);
	//----------------
	/*IceCream*/
	Matrix4x4 coneTransform;
	coneTransform = coneTransform.translate(Vector3D(0,-1.2,2));
	Matrix4x4 coneRotation;
	coneRotation = coneRotation.rotate(3.14, Vector3D(1,0,0));
	Shape *cone = new Cone(0.32, 1, 0.4, 1, coneTransform*coneRotation, brown);
	Matrix4x4 sphereTransform3;
	sphereTransform3 = Matrix4x4::translate(Vector3D(0,0,2));
	Shape *s3 = new Sphere(0.3, sphereTransform3, red_100);
	//-----------------
	/*Mirrored Ellipsoide inside a transmissive Cone*/
	Matrix4x4 cylinderTransform2;
	cylinderTransform2 = cylinderTransform2.translate(Vector3D(-0.8, 0, 1));
	Shape *c2 = new Cylinder(0.3, 0.7, cylinderTransform2, transmissive);
	Matrix4x4 elipsoideTransform1;
	elipsoideTransform1 = elipsoideTransform1.translate(Vector3D(-0.8, 0, 1));
	Shape *e1 = new Elipsoide(0.1,0.5, 0.3, elipsoideTransform1, mirror);
	//-----------------
	/*Cylinders at the bottom corners*/
	Matrix4x4 cylinderTransform3;
	cylinderTransform3 = cylinderTransform3.translate(Vector3D(-4.5,-2, 8));
	Shape *c3 = new Cylinder(0.5, 0.8, cylinderTransform3, mirror);
	Matrix4x4 cylinderTransform4;
	cylinderTransform4 = cylinderTransform4.translate(Vector3D(4.5, -2, 8));
	Shape *c4 = new Cylinder(0.5, 0.8, cylinderTransform4, mirror);
	/*Cylinders at the up corners*/
	Matrix4x4 cylinderTransform5;
	cylinderTransform5 = cylinderTransform5.translate(Vector3D(-4.6, 2.7, 8));
	Shape *c5 = new Cylinder(0.5, 0.2, cylinderTransform5, groc);
	Matrix4x4 cylinderTransform6;
	cylinderTransform6 = cylinderTransform6.translate(Vector3D(4.6, 2.7, 8));
	Shape *c6 = new Cylinder(0.5, 0.2, cylinderTransform6, groc);
	/*Cylinders as suporters*/
	Matrix4x4 cylinderTransform7;
	cylinderTransform7 = cylinderTransform7.translate(Vector3D(-4.7, 0, 8));
	Shape *c7 = new Cylinder(0.1, 3, cylinderTransform7, greyDiffuse);
	Matrix4x4 cylinderTransform8;
	cylinderTransform8 = cylinderTransform8.translate(Vector3D(4.7, 0, 8));
	Shape *c8 = new Cylinder(0.1, 3, cylinderTransform8, greyDiffuse);
	//-----------------
	/* Elipsoide rotation*/
	Matrix4x4 elRotation;
	elRotation = elRotation.rotate(3.14/2, Vector3D(0,0,1));
	Matrix4x4 elipsoideTransform2 = elipsoideTransform2.translate(Vector3D(1,0.9,3));
	Shape *e2 = new Elipsoide(0.3, 0.5, 0.3, elipsoideTransform2*elRotation, orange);
	//-----------------
	/* Hat-cone*/
	Matrix4x4 coneTransform2;
	coneTransform2 = coneTransform2.translate(Vector3D(1, 1.5, 2.8));
	Matrix4x4 coneRotation2;
	coneRotation2 = coneRotation2.rotate(0, Vector3D(1, 0, 0));
	Shape *cone2 = new Cone(1.5, 1, 0.4, 0.45, coneTransform2*coneRotation2, groc);
	//-----------------
	/* Cube Placement*/
	//Create CUBE 1
	double costat = 0.5;
	double height = costat / 2;
	Vector3D p0Front(1.1,-1.1, 1.5); //center of the front square
	Vector3D p0Back(p0Front.x, p0Front.y, p0Front.z + costat); //center of the back square
	Vector3D p0Left(p0Front.x - height, p0Front.y, p0Front.z + height); //center of the left square
	Vector3D p0Right(p0Front.x + height, p0Front.y, p0Front.z + height); //center of the right square
	Vector3D p0Bottom(p0Front.x, p0Front.y - height, p0Front.z + height); //center of the bottom square
	Vector3D p0Up(p0Front.x, p0Front.y + height, p0Front.z + height); //center of the up square

	Matrix4x4 frontTransform;
	frontTransform = Matrix4x4::translate(p0Front);
	Matrix4x4 backTransform;
	backTransform = Matrix4x4::translate(p0Back);
	Matrix4x4 leftTransform;
	leftTransform = Matrix4x4::translate(p0Left);
	Matrix4x4 rightTransform;
	rightTransform = Matrix4x4::translate(p0Right);
	Matrix4x4 bottomTransform;
	bottomTransform = Matrix4x4::translate(p0Bottom);
	Matrix4x4 upTransform;
	upTransform = Matrix4x4::translate(p0Up);
	// Planes to create the cube
	Shape *front = new Cube(height, frontTransform, p0Front, Vector3D(0, 0, 1), red_100);
	Shape *back = new Cube(height, backTransform, p0Back, Vector3D(0, 0, 1), bluesky);
	Shape *left = new Cube(height, leftTransform, p0Left, Vector3D(-1, 0, 0), bluesky);
	Shape *right = new Cube(height, rightTransform, p0Right, Vector3D(1, 0, 0), bluesky);
	Shape *bottom = new Cube(height, bottomTransform, p0Bottom, Vector3D(0, -1, 0), bluesky);
	Shape *up = new Cube(height, upTransform, p0Up, Vector3D(0, 1, 0), mirror);
	// Define a sphere inside the cube
	Matrix4x4 sphereTransform4;
	sphereTransform4 = sphereTransform4.translate(Vector3D(1.1,-1.1,1.2));
	Shape *s4 = new Sphere(0.1, sphereTransform4, red_100);
	//Create CUBE 2
	double costat2 =0.6;
	double height2 = costat2 / 2;
	Vector3D p0Front2(-0.8,-1.2, 1); //center of the front square
	Vector3D p0Back2(p0Front2.x, p0Front2.y, p0Front2.z + costat2); //center of the back square
	Vector3D p0Left2(p0Front2.x - height2, p0Front2.y, p0Front2.z + height2); //center of the left square
	Vector3D p0Right2(p0Front2.x + height2, p0Front2.y, p0Front2.z + height2); //center of the right square
	Vector3D p0Bottom2(p0Front2.x, p0Front2.y - height2, p0Front2.z + height2); //center of the bottom square
	Vector3D p0Up2(p0Front2.x, p0Front2.y + height2, p0Front2.z + height2); //center of the up square

	Matrix4x4 frontTransform2;
	frontTransform2 = Matrix4x4::translate(p0Front2);
	Matrix4x4 backTransform2;
	backTransform2 = Matrix4x4::translate(p0Back2);
	Matrix4x4 leftTransform2;
	leftTransform2 = Matrix4x4::translate(p0Left2);
	Matrix4x4 rightTransform2;
	rightTransform2 = Matrix4x4::translate(p0Right2);
	Matrix4x4 bottomTransform2;
	bottomTransform2 = Matrix4x4::translate(p0Bottom2);
	Matrix4x4 upTransform2;
	upTransform2 = Matrix4x4::translate(p0Up2);
	// Planes to create the cube
	Shape *front2 = new Cube(height2, frontTransform2, p0Front2, Vector3D(0, 0, -1), red_100);
	Shape *back2 = new Cube(height2, backTransform2, p0Back2, Vector3D(0, 0, -1), green);
	Shape *left2 = new Cube(height2, leftTransform2, p0Left2, Vector3D(-1, 0, 0), groc);
	Shape *right2 = new Cube(height2, rightTransform2, p0Right2, Vector3D(1, 0, 0), bluesky);
	Shape *bottom2 = new Cube(height2, bottomTransform2, p0Bottom2, Vector3D(0, -1, 0), bluesky);
	Shape *up2 = new Cube(height2, upTransform2, p0Up2, Vector3D(0, 1, 0), mirror);


	// Push all the objects to the scene
	objectsList->push_back(cone);
	objectsList->push_back(e1);
	objectsList->push_back(e2);
	objectsList->push_back(cone2);
	objectsList->push_back(c1);
	objectsList->push_back(c2);
	objectsList->push_back(c3);
	objectsList->push_back(c4);
	objectsList->push_back(c5);
	objectsList->push_back(c6);
	objectsList->push_back(c7);
	objectsList->push_back(c8);
	objectsList->push_back(s1);
	objectsList->push_back(s3);
	objectsList->push_back(s4);
	//Cube 1:
	objectsList->push_back(front); 
	objectsList->push_back(back); 
	objectsList->push_back(left); 
	objectsList->push_back(right); 
	objectsList->push_back(bottom);
	objectsList->push_back(up);
	//Cube 2:
	objectsList->push_back(front2);
	objectsList->push_back(back2);
	objectsList->push_back(left2);
	objectsList->push_back(right2);
	objectsList->push_back(bottom2);
	objectsList->push_back(up2);


	/* ****** */
	/* Lights */
	/* ****** */
	lightSourceList = new std::vector<PointLightSource>;
	Vector3D lightPosition1 = Vector3D(0, offset - 1, 2 * offset);
	Vector3D lightPosition2 = Vector3D(0, offset - 1, 0);
	Vector3D lightPosition3 = Vector3D(0, offset - 1, offset);
	Vector3D lightPosition4 = Vector3D(-1, 5, 0.5);


	Vector3D intensity = Vector3D(10, 10, 10); // Radiant intensity (watts/sr)
	PointLightSource pointLS1(lightPosition1, intensity);
	PointLightSource pointLS2(lightPosition2, intensity);
	PointLightSource pointLS3(lightPosition3, intensity);
	PointLightSource pointLS4(lightPosition4, Vector3D(5,5,5));

	lightSourceList->push_back(pointLS1);
	lightSourceList->push_back(pointLS2);
	lightSourceList->push_back(pointLS3);
	lightSourceList->push_back(pointLS4);
	
}

void raytrace(Camera* &cam, Shader* &shader, Film* &film,
              std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
    unsigned int sizeBar = 40;

    size_t resX = film->getWidth();
    size_t resY = film->getHeight();

    // Main raytracing loop
    // Out-most loop invariant: we have rendered lin lines
    for(size_t lin=0; lin<resY; lin++)
    {
        // Show progression
        if ( lin%(resY/sizeBar) == 0)
            std::cout << ".";

        // Inner loop invariant: we have rendered col columns
        for(size_t col=0; col<resX; col++)
        {
            // Compute the pixel position in NDC
            double x = (double)(col + 0.5) / resX;
            double y = (double)(lin + 0.5) / resY;

            // Generate the camera ray
            Ray cameraRay = cam->generateRay(x, y);

            // Compute ray color according to the used shader
            Vector3D pixelColor = shader->computeColor( cameraRay, *objectsList, *lightSourceList );

            // Store the pixel color
            film->setPixelValue(col, lin, pixelColor);
        }
    }
}

int main()
{
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";
    std::cout << separator << "RTIS - Ray Tracer for \"Imatge Sintetica\"" << separator << std::endl;

    // Create an empty film
    Film *film;
    film = new Film(720, 576);


    // Declare the shader
    Vector3D bgColor(0.0, 0.0, 0.0); // Background color (for rays which do not intersect anything)
    Vector3D intersectionColor(1,0,0); //red
	//Shader *shader = new IntersectionShader (intersectionColor, bgColor);
	//Shader *shader = new DepthShader(Vector3D(0.4, 1, 0.4), 10, bgColor);
	//Shader *shader = new NormalShader(bgColor);

	Shader *shader = new DirectShader(bgColor);
	//Shader *shader = new GlobalShader(bgColor,2);

    // Declare pointers to all the variables which describe the scene
    Camera *cam;
    std::vector<Shape*> *objectsList;
    std::vector<PointLightSource> *lightSourceList;

    // Build the scene
	newScene(cam, film, objectsList, lightSourceList);

    // Launch some rays!
    raytrace(cam, shader, film, objectsList, lightSourceList);

    // Save the final result to file
    std::cout << "\n\nSaving the result to file output.bmp\n" << std::endl;
    film->save();

    std::cout << "\n\n" << std::endl;
    return 0;
}
