#include "Object.hpp"
#include "Camera.hpp"
#include "Error.hpp"
using namespace std;

Object::Object(){
}

Object::~Object(){
    
}


// Initialization of object as a 'quad'
//
// This could be called in the constructor or
// otherwise 'explicitly' called this
// so we create our objects at the correct time
void Object::MakeTexturedQuad(std::string fileName){

       // Setup geometry
        // We are using a new abstraction which allows us
        // to create triangles shapes on the fly
        // Position and Texture coordinate 
        GLfloat tokenValue;

        ifstream inFile;
        inFile.open(fileName);
        SDL_Log("File opened");

        if(!inFile) {
                SDL_Log("Failed to open file");
                return;
        }

        //file reading
        string line;
        string newString;
        string mtlFileName;

        vector<tuple<int,int,int>> combination;

        string slash = "/";
        getline(inFile, line);
        cout << line;

        while(getline(inFile, line)) {
                if (line[0] == '#') {
                        continue;
                }

                istringstream str(line);
                string token;

                while(str >> token) {
                        if (token == "mtllib") {
                                str >> mtlFileName;
                        }

                        if(token == "v") {
                                for(int i=0; i<3; ++i) {
                                        str >> tokenValue;
                                        // SDL_Log(tokenValue);
                                        vertices.emplace_back(tokenValue);
                                }
                        }else if(token == "vt") {
                                for(int i=0; i<2; ++i) {
                                        str >> tokenValue;
                                        // SDL_Log(tokenValue);
                                        textures.emplace_back(tokenValue);
                                }

                        }else if(token == "vn") {
                                for(int i=0; i<3; ++i) {
                                        str >> tokenValue;
                                        // SDL_Log(tokenValue);
                                        normals.emplace_back(tokenValue);
                                }
                        }else if(token == "f") {
                                for(int i=0; i<3; ++i) {
                                        
                                    str >> newString;
                                    
                                    //read from 0 to / for v
                                    size_t pos = 0;
                                    string vertIndex;
                                    string texIndex;
                                    string normIndex;

                                    pos = newString.find(slash);
                                    // SDL_Log("Load vert index");
                                    vertIndex = newString.substr(0, pos);
                                    // SDL_Log(vertIndex.data());
                                    //erase the read part
                                    newString.erase(0, pos + slash.length());

                                    //read from / to / for vt
                                    pos = newString.find(slash);
                                    // SDL_Log("Load texture index");
                                    texIndex = newString.substr(0, pos);
                                    // SDL_Log(texIndex.data());
                                    //erase the read part
                                    newString.erase(0, pos + slash.length());

                                    //read form / to end for vn
                                    pos = newString.find(slash);
                                    // SDL_Log("Load texture index");
                                    normIndex = newString.substr(0, pos);
                                    // SDL_Log(texIndex.data());
                                    //erase the read part
                                    newString.erase(0, pos + slash.length());

                                    int ind, k;
                                    int num_vertIndex = stoi(vertIndex);
                                    int num_texIndex = stoi(texIndex);
                                    int num_normIndex = stoi(normIndex);

                                    tuple<int,int,int> newCombo = make_tuple(num_vertIndex, num_texIndex, num_normIndex);
                                    for(k=0; k<combination.size(); ++k) {
                                        if(combination[k] == newCombo) {
                                            ind = k;
                                            break;
                                        }
                                    }
                                    if(k == combination.size()) {
                                        m_geometry.AddVertex(vertices[(num_vertIndex-1)*3+0], vertices[(num_vertIndex-1)*3+1], vertices[(num_vertIndex-1)*3+2], 
                                                            textures[(num_texIndex-1)*2+0], textures[(num_texIndex-1)*2+1],
                                                            normals[(num_normIndex-1)*3+0], normals[(num_normIndex-1)*3+1], normals[(num_normIndex-1)*3+2]);
                                        combination.push_back(newCombo);
                                        ind = combination.size()-1;
                                    }
                                    indices.push_back(ind);
                                }
                        }
                }
        }

        for (int i = 0; i < indices.size(); i += 3) {
            m_geometry.MakeTriangle(indices[i], indices[i+1], indices[i+2]);
        }

        m_geometry.Gen();

        // Create a buffer and set the stride of information
        // NOTE: How we are leveraging our data structure in order to very cleanly
        //       get information into and out of our data structure.
        m_vertexBufferLayout.CreateNormalBufferLayout(m_geometry.GetBufferDataSize(),
                                        m_geometry.GetIndicesSize(),
                                        m_geometry.GetBufferDataPtr(),
                                        m_geometry.GetIndicesDataPtr());

        // Load our actual texture
        // We are using the input parameter as our texture to load
        int rootUntil = GetRootPath(fileName);
        LoadTexture(fileName.substr(0, rootUntil), mtlFileName);
        
        // Setup shaders
        std::string vertexShader = m_shader.LoadShader("./shaders/vert.glsl");
        std::string fragmentShader = m_shader.LoadShader("./shaders/frag.glsl");
        // Actually create our shader
        m_shader.CreateShader(vertexShader,fragmentShader);

    inFile.close();
}

int Object::GetRootPath (std::string& path) {
    char delim = '/';
    int delimStartFrom = 0;
    int result = 0;

    while (result = path.find(delim, delimStartFrom)) {
        if (result == std::string::npos) break;
        delimStartFrom = result + 1;
    }
    return delimStartFrom;
}

// TODO: In the future it may be good to 
// think about loading a 'default' texture
// if the user forgets to do this action!
void Object::LoadTexture(std::string pathRoot, std::string mtlFileName){
    // open a file
    std::ifstream infile;
    std::cout << "texture file is: " << pathRoot << mtlFileName << std::endl;
    infile.open(pathRoot + mtlFileName);
    // check if file loaded
    if (!infile)
    {
        std::cout << "Failed to open the texture file, nothing read." << std::endl;
        return;
    }

    std::string line;
    std::getline(infile, line);
    std::string texFile;
    int dataPos = 0;
    while (std::getline(infile, line))
    {
        // skip all comments
        if (line[0] == '#') 
        {
            continue;
        }
        // make the current line an input stream
        std::istringstream iss(line);
        std::string lineType;
        iss >> lineType;

        // reading texture info
        if (lineType == "map_Kd")
        {
            iss >> texFile;
        }
    }
    infile.close();
    // Load our actual textures
    m_textureDiffuse.LoadTexture(pathRoot + texFile);
}

// Bind everything we need in our object
// Generally this is called in update() and render()
// before we do any actual work with our object
void Object::Bind(){
        // Make sure we are updating the correct 'buffers'
        m_vertexBufferLayout.Bind();
        // Diffuse map is 0 by default, but it is good to set it explicitly
        m_textureDiffuse.Bind(0);
        // Select our appropriate shader
        m_shader.Bind();
}

void Object::Update(unsigned int screenWidth, unsigned int screenHeight){
        // Call our helper function to just bind everything
        Bind();
        // TODO: Read and understand
        // For our object, we apply the texture in the following way
        // Note that we set the value to 0, because we have bound
        // our texture to slot 0.
        m_shader.SetUniform1i("u_DiffuseMap",0);  
         // Here we apply the 'view' matrix which creates perspective.
        // The first argument is 'field of view'
        // Then perspective
        // Then the near and far clipping plane.
        // Note I cannot see anything closer than 0.1f units from the screen.
        // TODO: In the future this type of operation would be abstracted away
        //       in a camera class.
        m_projectionMatrix = glm::perspective(45.0f,((float)screenWidth)/((float)screenHeight),0.1f,20.0f);

        // Set the uniforms in our current shader
        // Set the MVP Matrix for our object
        // Send it into our shader
        m_shader.SetUniformMatrix4fv("model", &m_transform.GetInternalMatrix()[0][0]);
        m_shader.SetUniformMatrix4fv("view", &Camera::Instance().GetWorldToViewmatrix()[0][0]);
        m_shader.SetUniformMatrix4fv("projection", &m_projectionMatrix[0][0]);

        // Create a first 'light'
        // Create a first 'light'
        // Set in a light source position
        m_shader.SetUniform3f("lightPos",0.0f,0.0f,0.0f);       
        // Set a view and a vector
        m_shader.SetUniform3f("viewPos",0.0f,0.0f,0.0f);

}

// Render our geometry
void Object::Render(){
    // Call our helper function to just bind everything
    Bind();
	//Render data
    glDrawElements(GL_TRIANGLES,
                   m_geometry.GetIndicesSize(), // The number of indicies, not triangles.
                   GL_UNSIGNED_INT,             // Make sure the data type matches
                        nullptr);               // Offset pointer to the data. 
                                                // nullptr because we are currently bound
}

// Returns the actual transform stored in our object
// which can then be modified
Transform& Object::GetTransform(){
    return m_transform; 
}
