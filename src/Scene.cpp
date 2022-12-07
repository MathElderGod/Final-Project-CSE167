/**************************************************
Scene.cpp contains the implementation of the draw command
*****************************************************/
#include "Scene.h"
#include "Cube.h"
#include "Obj.h"

// The scene init definition 
#include "Scene.inl"


using namespace glm;
void Scene::draw(void) {
    // Pre-draw sequence: assign uniforms that are the same for all Geometry::draw call.  These uniforms include the camera view, proj, and the lights.  These uniform do not include modelview and material parameters.
    realCamera->computeMatrices();
    shader->view = realCamera->view;
    shader->lightview = lightCamera->view;
    shader->projection = realCamera->proj;
    shader->lightproj = lightCamera->proj;
    shader->nlights = light.size();
    shader->lightpositions.resize(shader->nlights);
    shader->lightcolors.resize(shader->nlights);
    int count = 0;
    for (std::pair<std::string, Light*> entry : light) {
        shader->lightpositions[count] = (entry.second)->position;
        shader->lightcolors[count] = (entry.second)->color;
        count++;
    }

    // Define stacks for depth-first search (DFS)
    std::stack < Node* > dfs_stack;
    std::stack < mat4 >  matrix_stack; // HW3: You will update this matrix_stack during the depth-first search while loop.

    // Initialize the current state variable for DFS
    Node* cur = node["world"]; // root of the tree
    mat4 cur_VM = realCamera->view; // HW3: You will update this current modelview during the depth first search.  Initially, we are at the "world" node, whose modelview matrix is just camera's view matrix.

    dfs_stack.push(cur);
    matrix_stack.push(cur_VM);
    // Compute total number of connectivities in the graph; this would be an upper bound for
    // the stack size in the depth first search over the directed acyclic graph
    int total_number_of_edges = 0;
    for (const auto& n : node) total_number_of_edges += n.second->childnodes.size();


    while (!dfs_stack.empty()) {
        // Detect whether the search runs into infinite loop by checking whether the stack is longer than the number of edges in the graph.
        if (dfs_stack.size() > total_number_of_edges) {
            std::cerr << "Error: The scene graph has a closed loop." << std::endl;
            exit(-1);
        }

        // top-pop the stacks
        cur = dfs_stack.top();  dfs_stack.pop();
        cur_VM = matrix_stack.top(); matrix_stack.pop();

        // draw all the models at the current node
        for (size_t i = 0; i < cur->models.size(); i++) {
            // Prepare to draw the geometry. Assign the modelview and the material.
            mat4 M = cur->modeltransforms[i];

            shader->modelview = cur_VM * M; // DONE: HW3: Without updating cur_VM, modelview would just be camera's view matrix.
            shader->material = (cur->models[i])->material;

            // The draw command
            shader->setUniforms();
            (cur->models[i])->geometry->draw();
        }

        // Continue the DFS: put all the child nodes of the current node in the stack
        for (size_t i = 0; i < cur->childnodes.size(); i++) {
            dfs_stack.push(cur->childnodes[i]);
            mat4 T = cur->childtransforms[i];
            matrix_stack.push(cur_VM * T);
        }

    }
}

void Scene::drawLightCameraView() {
    // Pre-draw sequence: assign uniforms that are the same for all Geometry::draw call.  These uniforms include the camera view, proj, and the lights.  These uniform do not include modelview and material parameters.
    lightCamera->computeLightCameraMatrices();
    depthShader->view = lightCamera->view;
    depthShader->projection = lightCamera->proj;

    // Define stacks for depth-first search (DFS)
    std::stack < Node* > dfs_stack;
    std::stack < mat4 >  matrix_stack; // HW3: You will update this matrix_stack during the depth-first search while loop.

    // Initialize the current state variable for DFS
    Node* cur = node["world"]; // root of the tree
    mat4 cur_VM = lightCamera->view; // HW3: You will update this current modelview during the depth first search.  Initially, we are at the "world" node, whose modelview matrix is just camera's view matrix.

    dfs_stack.push(cur);
    matrix_stack.push(cur_VM);
    // Compute total number of connectivities in the graph; this would be an upper bound for
    // the stack size in the depth first search over the directed acyclic graph
    int total_number_of_edges = 0;
    for (const auto& n : node) total_number_of_edges += n.second->childnodes.size();

    while (!dfs_stack.empty()) {
        // Detect whether the search runs into infinite loop by checking whether the stack is longer than the number of edges in the graph.
        if (dfs_stack.size() > total_number_of_edges) {
            std::cerr << "Error: The scene graph has a closed loop." << std::endl;
            exit(-1);
        }

        // top-pop the stacks
        cur = dfs_stack.top();  dfs_stack.pop();
        cur_VM = matrix_stack.top(); matrix_stack.pop();

        // draw all the models at the current node
        for (size_t i = 0; i < cur->models.size(); i++) {
            // Prepare to draw the geometry. Assign the modelview and the material.
            mat4 M = cur->modeltransforms[i];

            depthShader->modelview = cur_VM * M; 

            // The draw command
            depthShader->setUniforms();
            (cur->models[i])->geometry->draw();
        }

        // Continue the DFS: put all the child nodes of the current node in the stack
        for (size_t i = 0; i < cur->childnodes.size(); i++) {
            dfs_stack.push(cur->childnodes[i]);
            mat4 T = cur->childtransforms[i];
            matrix_stack.push(cur_VM * T);
        }
    }
}