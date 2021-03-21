#include "ui.h"

void UI::addIcons() {   
    //icons from https://osirisnewdawn.gamepedia.com/Crafting_materials
    //Resourcen
  LoadTextureFromFile(ASSET_DIRECTORY "icons/phosphor.png", &iconPhosphor, &my_image_width, &my_image_height);
  LoadTextureFromFile(ASSET_DIRECTORY "icons/titan.png", &iconTitan, &my_image_width, &my_image_height);
  LoadTextureFromFile(ASSET_DIRECTORY "icons/kohlenstoff.png", &iconKohlenstoff, &my_image_width, &my_image_height);
  LoadTextureFromFile(ASSET_DIRECTORY "icons/eisen.png", &iconEisen, &my_image_width, &my_image_height);
  LoadTextureFromFile(ASSET_DIRECTORY "icons/gold.png", &iconGold, &my_image_width, &my_image_height);
  LoadTextureFromFile(ASSET_DIRECTORY "icons/bronze.png", &iconBronze, &my_image_width, &my_image_height);
  LoadTextureFromFile(ASSET_DIRECTORY "icons/platin.png", &iconPlatin, &my_image_width, &my_image_height);
  LoadTextureFromFile(ASSET_DIRECTORY "icons/plutonium.png", &iconPlutionium, &my_image_width, &my_image_height);
  LoadTextureFromFile(ASSET_DIRECTORY "icons/kohle.png", &iconStaub, &my_image_width, &my_image_height);
  //
  LoadTextureFromFile(ASSET_DIRECTORY "icons/credit.png", &iconCredit, &my_image_width, &my_image_height);
  LoadTextureFromFile(ASSET_DIRECTORY "icons/shield.png", &iconShild, &my_image_width, &my_image_height);
  LoadTextureFromFile(ASSET_DIRECTORY "icons/shield_full.png", &iconShild_full, &my_image_width, &my_image_height);
  LoadTextureFromFile(ASSET_DIRECTORY "icons/helth.png", &iconLeben, &my_image_width, &my_image_height);
  //https://osirisnewdawn.gamepedia.com/Ammunition
  LoadTextureFromFile(ASSET_DIRECTORY "icons/LaserIcon.png", &iconMunition, &my_image_width, &my_image_height);
}
void UI::updateUI(const float mshild, const float mleben, const int mmunition, const int mmaxAmmo, const float mcredit, const float mtreibstoff,  map<Resource*, int> cargo ,list<Asteroid*> masteroids){
    shild = mshild;
    leben = mleben;
    munition = mmunition;
    credit = mcredit;
    maxAmmo = mmaxAmmo;
    treibstoff = mtreibstoff;
    asteroids = masteroids;
   
    for (map<Resource*, int>::iterator it = cargo.begin(); it != cargo.end(); ++it){
#ifdef DEBUG
        
#endif // DEBUG    
       if (it->first->getName() == "Kohlenstoff") kohlenstoff = it->second; 
       if (it->first->getName() == "Plutonium") plutonium = it->second;
       if (it->first->getName() == "Phosphor")phosphor = it->second; 
       if (it->first->getName() == "Eisen")eisen = it->second; 
       if (it->first->getName() == "Gold") gold = it->second; 
       if (it->first->getName() == "Bronze") bronze = it->second; 
       if (it->first->getName() == "Titan") titan = it->second; 
       if (it->first->getName() == "Platin")platin = it->second; 
       if (it->first->getName() == "Asteroidenstaub") {
           asteroidenstaub = it->second; 
       }
    }
}
void UI::PrintMenu() {
    
    static bool open = true;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (open) {
        ImVec2 window_pos = ImVec2(io.DisplaySize.x/2+50,300);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(1.0f, 1.0f));
        if (!ImGui::Begin("Test window", &open, window_flags)) {
            ImGui::End();
        }
        else {
            ImGui::TextColored(ImVec4(255, 255, 0, 1), "SPACE RACER!");
            if (ImGui::Button("Spiel Starten")) {
                open = false;
            }
            if (ImGui::Button("Spiel Beenden")) {
                open = false;
            }
            ImGui::End();
        }
    }
    else {
        PrintUI();
    }
}
void UI::PrintUI() {
  
   //ImGui::ShowDemoWindow();
 
    const float DISTANCE = 10.0f;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    static int Box1corner = 0;
    if (Box1corner != -1)
    {
        window_flags |= ImGuiWindowFlags_NoMove;
        ImVec2 window_pos = ImVec2((Box1corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (Box1corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((Box1corner & 1) ? 1.0f : 0.0f, (Box1corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    }
   
     const float SIZE = 16;
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("O1", NULL, window_flags))
    {        
        //iconShild, iconLeben, iconMunition, iconKohlenstoff, iconEisen, iconGold, iconBronze, iconPlatin, iconPlutionium, iconTitan
        ImGui::Image((void*)(intptr_t)iconKohlenstoff, ImVec2(SIZE, SIZE));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("%u |", kohlenstoff);
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Image((void*)(intptr_t)iconPhosphor, ImVec2(SIZE, SIZE));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("%u |", phosphor);
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Image((void*)(intptr_t)iconEisen, ImVec2(SIZE, SIZE));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("%u |", eisen); 
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Image((void*)(intptr_t)iconGold, ImVec2(SIZE, SIZE));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("%u |", gold);
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Image((void*)(intptr_t)iconBronze, ImVec2(SIZE, SIZE));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("%u |", bronze);
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Image((void*)(intptr_t)iconTitan, ImVec2(SIZE, SIZE));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("%u |", titan);
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Image((void*)(intptr_t)iconPlatin, ImVec2(SIZE, SIZE));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("%u |", platin);
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Image((void*)(intptr_t)iconPlutionium, ImVec2(SIZE, SIZE));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("%u |", plutonium);
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Image((void*)(intptr_t)iconStaub, ImVec2(SIZE, SIZE));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("%u ", asteroidenstaub);
       // ImGui::Text("K:  %u |P:  %u | P: % u | E:  %u |G:  %u |B:  %u | T:  %u | P:  %u ", kohlenstoff, phosphor, eisen, gold, bronze, titan, platin, plutionium);
    }
    ImGui::End();
    static int tankBox =2;
    if (tankBox != -1)
    {
        window_flags |= ImGuiWindowFlags_NoMove;
        ImVec2 window_pos = ImVec2((tankBox & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (tankBox & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((tankBox & 1) ? 1.0f : 0.0f, (tankBox & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    }
    
    if (ImGui::Begin("tank", NULL, window_flags))
    {
        float progress;
        progress = treibstoff/maxTreibstoff;
      //  cout << progress << endl;
        ImGui::Text("Treibstoff");
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
    }
    ImGui::End();
    static int Box2corner = 3;
    if (Box2corner != -1)
    {
        window_flags |= ImGuiWindowFlags_NoMove;
        ImVec2 window_pos = ImVec2((Box2corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (Box2corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((Box2corner & 1) ? 1.0f : 0.0f, (Box2corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    }
   
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("O2", NULL, window_flags))
    {
        ImGui::Image((void*)(intptr_t)iconCredit, ImVec2(16, 16));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("Credit:  %u", credit);
        ImGui::Separator();
        ImGui::Image((void*)(intptr_t) (shild >= 1 ? iconShild_full : iconShild), ImVec2(16, 16));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("Schild:  %.1f \%",shild );
        ImGui::Separator();
        ImGui::Image((void*)(intptr_t)iconLeben, ImVec2(16, 16));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("Leben:  %.1f \%", leben);
        ImGui::Separator();
        ImGui::Image((void*)(intptr_t)iconMunition, ImVec2(16, 16));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("Munition: %u/%u", munition,maxAmmo );
    }
    ImGui::End();
   
    static int KompassCorner = 1;
    if (KompassCorner != -1)
    {
        window_flags |= ImGuiWindowFlags_NoMove;
        ImVec2 window_pos = ImVec2((KompassCorner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (KompassCorner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((KompassCorner & 1) ? 1.0f : 0.0f, (KompassCorner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Kompass", NULL, window_flags))
    {
        ImGui::Text("Position: ");
        ImGui::Separator();
        ImGui::Text(" x%.1f",  pos.X);
        ImGui::Text(" y:%.1f", pos.Y);
        ImGui::Text(" z:%.1f", pos.Z);
        ImGui::Separator();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
        ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
        if (ImGui::BeginChild("canvas", ImVec2(125.0f, 125.0f), true, window_flags)) {
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImVec2 window_pos = ImGui::GetWindowPos();
            ImVec2 window_size = ImGui::GetWindowSize();
            ImVec2 window_center = ImVec2(window_pos.x  +  window_size.x * 0.5f, window_pos.y +  window_size.y * 0.5f);
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            //void ImDrawList::Addircle(const ImVec2& center, float radius, ImU32 col, int num_segments, float thickness)
            draw_list->AddCircle(ImVec2(window_center.x , window_center.y ), 2, IM_COL32(0, 0, 255, 255), 1, 5.0f);
            for (list<Asteroid*>::iterator it =asteroids.begin(); it != asteroids.end(); ++it)
            {
              //Übergeben von Asteroiden Kooridaten
          
              Vector v = (*it)->transform().translation();
           
              //200 radius vom schiff
             if((v.X < pos.X+200 && v.X > pos.X - 200) || (v.Z < pos.Z + 200 && v.Z > pos.Z - 200))
              //    cout << v.X << "-" << v.Z << endl;
                  draw_list->AddCircle(ImVec2(window_center.x - v.X + pos.X,window_center.y + v.Z + pos.Z) , 2, IM_COL32(255, 0, 0, 255), 1, 5.0f);
            }
            
        }
        ImGui::EndChild();
    }
    ImGui::End();
  
}

    /*
void UI::PrintMenu() {
    static bool open = true;
    const float DISTANCE = 10.0f;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    static int tankBox = 0;
    if (tankBox != -1)
    {
        window_flags |= ImGuiWindowFlags_NoMove;
        ImVec2 window_pos = ImVec2((tankBox & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (tankBox & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((tankBox & 1) ? 1.0f : 0.0f, (tankBox & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    }
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2 - 75, io.DisplaySize.y / 3), ImGuiCond_FirstUseEver);
    if (open) {
        if (ImGui::Begin("menu", NULL, window_flags)){
            ImGui::TextColored(ImVec4(204, 207, 70, 1), "SPACE RACER!");
            if (ImGui::Button("Start the Game")) {
                open = false;
            }
           
        }
    }
  */

void UI::SelectPlay() {
    PrintUI();
}
void UI::SelectBeenden() {

}
void UI::displayNotification(int notification) {
    const float DISTANCE = 10.0f;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        window_flags |= ImGuiWindowFlags_NoMove;
        ImVec2 window_pos = ImVec2(300,200);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(1.0f,1.0f));
  
   
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("notification", NULL, window_flags)){
        switch (notification)
        {
        case CARGO_SPACE_LOW:
            ImGui::TextColored(ImVec4(255, 0, 0, 1), "Der Stauraum ist voll.");
            ImGui::Separator();
            break;
        case AMMO_LOW:
            ImGui::TextColored(ImVec4(255, 0, 0, 1), "die munition ist fast alle");
            ImGui::Separator();
            break;
        case SHIELD_LOW:
            ImGui::TextColored(ImVec4(255, 0, 0, 1), "dein Shild ist fast kapput");
            ImGui::Separator();
            break;
        case HEALTH_LOW:
            ImGui::TextColored(ImVec4(255, 0, 0, 1), "Das leben ist kurz, sei vorsichtig!");
            ImGui::Separator();
            break;
        }
    }
    ImGui::End();
 
}
void UI::displayGameOver() {
    const float DISTANCE = 10.0f;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    window_flags |= ImGuiWindowFlags_NoMove;
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();
    ImVec2 window_center = ImVec2(window_pos.x + window_size.x * 0.8f, window_pos.y + window_size.y * 0.5f);
    ImGui::SetNextWindowPos(window_center, ImGuiCond_Always, ImVec2(1.0f, 1.0f));


    ImGui::SetNextWindowBgAlpha(0.35f);
    if (ImGui::Begin("GameOver", NULL, window_flags)) {
        ImGui::SetWindowFontScale(2);
        ImGui::TextColored(ImVec4(255, 0, 0, 1), "Game Over");
    }
    ImGui::End();
}
/** funktion von: https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#Example-for-OpenGL-users
* @param filename Der Dateipfad, 
* @param out_texture speicherort in GLuinit format
* @param out_width  breite des gespeicherten bildes
* @param out_height höhe des gespeicherten bildes
*/
bool UI::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
   // cout << filename << "-" << endl;
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // Setup filtering parameters for display
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
  
    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}