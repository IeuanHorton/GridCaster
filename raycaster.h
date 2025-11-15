void drawPlayer();

float distanceOfRay(float ax, float ay, float bx, float by, float angle);

float depthOfFieldCheck(int depthOfField, float xOffset, float yOffset, Ray* ray);
void horizontalLineCheck(Ray* ray);
void verticalLineCheck(Ray* ray);
void drawRays();
void display();
void buttons(unsigned char key, int x, int y);
void init();
void FixFishEye(Ray* ray);
bool LookingUp(Ray* ray);
bool LookingDown(Ray* ray);
bool LookingRight(Ray* ray);
bool LookingLeft(Ray* ray);
