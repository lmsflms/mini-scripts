#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//Fonction callback custom appellée dès qu'il y a un évenement changement de taille
//qui met à jour le viewport avec des nouvelles tailles de fenêtres.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	int WINDOW_WIDTH = 800;
	int WINDOW_HEIGHT = 600;

	//================= MISE EN PLACE DE GLFW =================
	
	//Initialisation de GLFW
	glfwInit();
	
	//Configuration des options de GLFW
	//(on utilise la version 3.3, d'où les entiers en arguments)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Crée un contexte en core profile (voir tout début du doc)
	//OpenGL 3.3 inclut encore l’immediate mode dans compatibility profile.. mais seulement pour faire tourner les vieux progs.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//================= CREATION D'UNE FENETRE =================

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Titre de la fenêtre", NULL, NULL);
	
	if (window == NULL) {
		std::cout << "Erreur à la création de la fenêtre GLFW\n";
		glfwTerminate();
		return -1;
	}

	//Binding de notre objet window
	glfwMakeContextCurrent(window);

	//================= GLAD =================
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//================= VIEWPORT =================

	//A la création de la fenêtre, on a défini sa taille mais pour GLFW,
	//il faut maintenant que OpenGL connaisse la taille.
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); //fonction d'état global

	//On binde note fonction callback custom "framebuffer_size_callback"
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//================= BOUCLE DE RENDU =================

	//L'image OpenGL est dessinée en continu tant que la fenêtre est ouverte et que l'utilisateur peut intéragir
	//Une itération de cette boucle est appellée frame.
	int frame = 0;
	while (!glfwWindowShouldClose(window)) //vérifie si la fenêtre doit se fermer
	{
		++frame;
		std::cout << "\033[2J\033[1;1H"; //clear la console
		std::cout << "Frame: " << frame;

		processInput(window); //check les touches

		//rendu

		//On modifie l’état global en changeant la couleur utilisée pour nettoyer le color buffer
		glClearColor(0.6f, 0.8f, 0.3f, 1.0f);
		//on nettoie le color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		//-----
		glfwSwapBuffers(window); //double buffering
		glfwPollEvents(); //vérifie si il y a de nouveaux évenements
	}

	glfwTerminate(); 
	return 0;
}

