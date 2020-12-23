
#include "Main.h"


uintptr_t GamePid = 0;
uintptr_t GameBaseAddress = 0;
uintptr_t entitylist = 0;

uintptr_t Spectators = 0;

uintptr_t nextAim = 0;
uintptr_t AimTarget = 0;
uintptr_t nextEntityInfoUpdate = 0;

float current_fov_limiter = 999.f;


bool printableOut = false;

//Selection for settings
//1 = public safe
//2 = 1v1 (friendly aimlock)
//3 = public aimlock (UNSAFE)
int choice = -1;
//Settings array, all set to off by default
int settings[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
//settings[0] = enable no recoil
//settings[1] = enable aimbot
//settings[2] = enable friendly aimbot
//settings[3] = enable aimbot lock mode
//settings[4] = disable aimbot with spectators
//settings[5] = disable aimbot lock with spectators
//settings[6] = count teammates as spectators
//settings[7] = enable glow

int enable_aimbot = 1;
int enable_norecoil = 1;
int enable_friendly_aimbot = 1;
int enable_aimbot_lock_mode = 1;
int disable_aimbot_with_spectators = 0;
int disable_aimbot_lock_mode_with_spectators = 1;
int count_team_entities_as_spectators = 1;
int enable_glow_hack = 1;

//Duplicate values for toggler
int og_enable_aimbot = 0;
int og_enable_norecoil = 0;
int og_enable_friendly_aimbot = 0;
int og_enable_aimbot_lock_mode = 0;
int og_disable_aimbot_with_spectators = 0;
int og_disable_aimbot_lock_mode_with_spectators = 0;
int og_count_team_entities_as_spectators = 0;
int og_enable_glow_hack = 0;

//visibility check
float vis_old[100];

//Toggle button status (cheat on by default)
//0 - off, 1 - on
int toggle_set = 1;

//Controller test variables
int button_pressed = 0;
int trigger_pressed = 0;


typedef bool (Entity::* EntityPtrDef)();
uintptr_t eptr(EntityPtrDef method) {
    return *(uintptr_t*)&method;
}

void LoadProtectedFunctions() {
    uintptr_t t = milliseconds_now();
    BYTE xorkey = 0x0;
    for (DWORD i = 0; i < 8; i++) {
        xorkey = ((BYTE*)&t)[i];
        if (xorkey > 0x3 && xorkey < 0xf0) {
            break;
        }
    }
    if (xorkey <= 0x3 || xorkey >= 0xf0) {
        xorkey = 0x56;
    }

    //Configures the protected functions of the program
    //A ProtectedFunction is address, size, lastXOR, crypted
    //For size it's next func minus current func - 0x3

    addFunc({ LoadProtectedFunctions, (uintptr_t)CheckDriverStatus - (uintptr_t)LoadProtectedFunctions - 0x3, xorkey, false });
    addFunc({ CheckDriverStatus, (uintptr_t)GetProcessIdByName - (uintptr_t)CheckDriverStatus - 0x3, xorkey, false });
    addFunc({ GetProcessIdByName, (uintptr_t)milliseconds_now - (uintptr_t)GetProcessIdByName - 0x3, xorkey, false });
    addFunc({ milliseconds_now, (uintptr_t)ProcessPlayer - (uintptr_t)milliseconds_now - 0x3, xorkey, false });
    addFunc({ ProcessPlayer, (uintptr_t)UpdatePlayersInfo - (uintptr_t)ProcessPlayer - 0x3, xorkey, false });
    addFunc({ UpdatePlayersInfo, (uintptr_t)PredictPosition - (uintptr_t)UpdatePlayersInfo - 0x3, xorkey, false });
    addFunc({ PredictPosition, (uintptr_t)AutoBoneSwitch - (uintptr_t)PredictPosition - 0x3, xorkey, false });
    addFunc({ AutoBoneSwitch, (uintptr_t)SmoothType_Asist - (uintptr_t)AutoBoneSwitch - 0x3, xorkey, false });
    addFunc({ SmoothType_Asist, (uintptr_t)SmoothType_TargetLock - (uintptr_t)SmoothType_Asist - 0x3, xorkey, false });
    addFunc({ SmoothType_TargetLock, (uintptr_t)AimAngles - (uintptr_t)SmoothType_TargetLock - 0x3, xorkey, false });
    addFunc({ AimAngles, (uintptr_t)RecoilDelete - (uintptr_t)AimAngles - 0x3, xorkey, false });
    addFunc({ RecoilDelete, (uintptr_t)CheatLoop - (uintptr_t)RecoilDelete - 0x3, xorkey, false });
    addFunc({ CheatLoop, (uintptr_t)Configure - (uintptr_t)CheatLoop - 0x3, xorkey, false });
    addFunc({ Configure, (uintptr_t)mainThread - (uintptr_t)Configure - 0x3, xorkey, false });
    addFunc({ mainThread, (uintptr_t)DllMain - (uintptr_t)mainThread - 0x3, xorkey, false });
    addFunc({ DllMain, (uintptr_t)std::_Narrow_char_traits<char, int>::eof - (uintptr_t)DllMain - 0x3, xorkey, false });


    addFunc({ Driver::SendCommand, (uintptr_t)Driver::GetBaseAddress - (uintptr_t)Driver::SendCommand - 0x3, xorkey, false });
    addFunc({ Driver::GetBaseAddress, (uintptr_t)Driver::copy_memory - (uintptr_t)Driver::GetBaseAddress - 0x3, xorkey, false });
    addFunc({ Driver::copy_memory, (uintptr_t)GetKernelModuleExport - (uintptr_t)Driver::copy_memory - 0x3, xorkey, false });
    addFunc({ GetKernelModuleExport, (uintptr_t)GetKernelModuleAddress - (uintptr_t)GetKernelModuleExport - 0x3, xorkey, false });
    addFunc({ GetKernelModuleAddress, (uintptr_t)Driver::initialize - (uintptr_t)GetKernelModuleAddress - 0x3, xorkey, false });
    addFunc({ Driver::initialize, (uintptr_t)Driver::read_memory - (uintptr_t)Driver::initialize - 0x3, xorkey, false });
    addFunc({ Driver::read_memory, (uintptr_t)Driver::write_memory - (uintptr_t)Driver::read_memory - 0x3, xorkey, false });
    addFunc({ Driver::write_memory, (uintptr_t)getEntity - (uintptr_t)Driver::write_memory - 0x3, xorkey, false });
    addFunc({ getEntity, (uintptr_t)paddingsub - (uintptr_t)getEntity - 0x3, xorkey, false });

    
    addFunc({ (void*)eptr(&Entity::isPlayer), eptr((EntityPtrDef)&Entity::getBonePosition) - eptr(&Entity::isPlayer) - 0x3, xorkey, false });
    addFunc({ (void*)eptr((EntityPtrDef)&Entity::getBonePosition), eptr((EntityPtrDef)&Entity::GetViewAngles) - eptr((EntityPtrDef)&Entity::getBonePosition) - 0x3, xorkey, false });
    addFunc({ (void*)eptr((EntityPtrDef)&Entity::Observing), eptr((EntityPtrDef)&Entity::getTeamId) - eptr((EntityPtrDef)&Entity::Observing) - 0x3, xorkey, false });
    
    for (size_t i = 0; i < funcCount; i++) {
        if (functions[i].address != LoadProtectedFunctions)
            Protect(functions[i].address);
    }
    Unprotect(_ReturnAddress());
}

bool CheckDriverStatus() {
    int icheck = 29;
    NTSTATUS status = 0;
    SLog(L"getting base");
    Unprotect(Driver::GetBaseAddress);
    uintptr_t BaseAddr = Driver::GetBaseAddress(Driver::currentProcessId);
    SLog(std::wstring(L"base: "+std::to_wstring(BaseAddr)).c_str());
    if (BaseAddr == 0) {
        return false;
    }
    Protect(Driver::GetBaseAddress);

    SLog(L"reading var");
    int checked = Driver::read<int>(Driver::currentProcessId, (uintptr_t)&icheck, &status);
    SLog(std::wstring(L"readed: " + std::to_wstring(checked)).c_str());
    if (checked != icheck) {
        return false;
    }
    
    return true;
}

DWORD GetProcessIdByName(wchar_t * name) {
    Protect(_ReturnAddress());

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE) {
        while (Process32Next(snapshot, &entry) == TRUE) {
            if (_wcsicmp(entry.szExeFile, name) == 0) {
                Unprotect(_ReturnAddress());
                return entry.th32ProcessID;
            }
        }
    }

    CloseHandle(snapshot);
    Unprotect(_ReturnAddress());
    return 0;
}



uintptr_t milliseconds_now() {
    static LARGE_INTEGER s_frequency;
    static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
    if (s_use_qpc) {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return (1000LL * now.QuadPart) / s_frequency.QuadPart;
    }
    else {
        return GetTickCount();
    }
}

struct GColor {
    float r, g, b;
};
struct GlowMode {
    BYTE GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};
struct Fade {
    int a, b;
    float c, d, e, f;
};

void ProcessPlayer(Entity* LPlayer, Entity* target, UINT64 entitylist, int id) {
    Protect(_ReturnAddress());
    auto fptr = &Entity::Observing;
    Unprotect((void*)*(uintptr_t*)&fptr);
    intptr_t obser = target->Observing(GamePid, entitylist);
    Protect((void*)*(uintptr_t*)&fptr);
    if (obser == LPlayer->ptr) {
        if (target->getTeamId() != LPlayer->getTeamId() ||
            target->getTeamId() == LPlayer->getTeamId() && settings[6] == 1) {
            Spectators++;
        }
    }
    if (obser != 0) { // Is an observer... nothing to do
        Unprotect(_ReturnAddress());
        return;
    }
    auto fptrBone = &Entity::getBonePosition;
    Unprotect((void*)*(uintptr_t*)&fptrBone);
    Vector BonePosition = target->getBonePosition(GamePid, 3);
    Protect((void*)*(uintptr_t*)&fptrBone);
    Vector LocalPlayerPosition = LPlayer->getPosition();
    float dist = LocalPlayerPosition.DistTo(BonePosition) / 39.62f;
    if (dist > Max_Cheat_Distance || BonePosition.z > 22000.f) {
        Unprotect(_ReturnAddress());
        return;
    }

    int health = target->getHealth();
    if (health < 0 || health > 100) {
        Unprotect(_ReturnAddress());
        return;
    }

    int entity_team = target->getTeamId();
    if (entity_team < 0 || entity_team>31) {
        Unprotect(_ReturnAddress());
        return;
    }

    if (settings[7] == 1) {
        if ((int)target->buffer[GLOW_CONTEXT] != 1 || (int)target->buffer[GLOW_VISIBLE_TYPE] != 1 || (int)target->buffer[GLOW_FADE] != 872415232) {
            float currentEntityTime = 5000.f;//(float)target->buffer[0xEE4];
            if (!isnan(currentEntityTime) && currentEntityTime > 0.f) {
                GlowMode mode = { 101,102,96,75 };
                GColor color;

                //Teammates - light blue
                if (target->getTeamId() == LPlayer->getTeamId()) {
                    color = { 0.f, 2.f, 3.f };
                }
                //Downed players - orange
                else if (target->isBleedOut() || !target->isOkLifeState()) {
                    color = { 3.f, 2.f, 0.f };
                }
                //Visible enemies - green
                else if (target->vis_time() > vis_old[id] || target->vis_time() < 0.f && vis_old[id] > 0.f) {
                    color = { 0.f, 2.f, 0.f };
                }
                //Non-visible enemies WITH spectators - purple
                else if (Spectators > 0) {
                    color = { 2.f, 0.f, 3.f };
                }
                //Non-visible enemies WITHOUT spectators - red
                else {
                    color = { 3.f, 0.f, 0.f };
                }
                //printf("Changed: %p\n", target->ptr);
                Driver::write<GlowMode>(GamePid, target->ptr + GLOW_TYPE, mode);
                Driver::write<GColor>(GamePid, target->ptr + GLOW_COLOR, color);

                Driver::write<float>(GamePid, target->ptr + GLOW_DISTANCE, 40000.f);
                Driver::write<float>(GamePid, target->ptr + GLOW_LIFE_TIME, currentEntityTime);
                currentEntityTime -= 1.f;
                Driver::write<int>(GamePid, target->ptr + GLOW_CONTEXT, 1); //This value should enable glow?
                Driver::write<int>(GamePid, target->ptr + GLOW_VISIBLE_TYPE, 1); //This value should enable glow?
                Driver::write<Fade>(GamePid, target->ptr + GLOW_FADE, { 872415232, 872415232, currentEntityTime, currentEntityTime, currentEntityTime, currentEntityTime });

            }
        }
    }

    //Disable glow hack
    if (settings[7] == 0 && toggle_set == 0) {
        if ((int)target->buffer[GLOW_CONTEXT] != 1 || (int)target->buffer[GLOW_VISIBLE_TYPE] != 1 || (int)target->buffer[GLOW_FADE] != 872415232) {
            float currentEntityTime = 5000.f;//(float)target->buffer[0xEE4];
            if (!isnan(currentEntityTime) && currentEntityTime > 0.f) {
                //Set color to black in case anything goes wrong
                GColor color;
                color = { 0.f, 0.f, 0.f };
                
                //Write glow disable values
                Driver::write<GColor>(GamePid, target->ptr + GLOW_COLOR, color);
                Driver::write<float>(GamePid, target->ptr + GLOW_DISTANCE, 0.f);
                Driver::write<float>(GamePid, target->ptr + GLOW_LIFE_TIME, currentEntityTime);
                currentEntityTime -= 1.f;
                Driver::write<int>(GamePid, target->ptr + GLOW_CONTEXT, 0); //This value should enable glow?
                Driver::write<int>(GamePid, target->ptr + GLOW_VISIBLE_TYPE, 0); //This value should enable glow?
                Driver::write<Fade>(GamePid, target->ptr + GLOW_FADE, { 872415232, 872415232, currentEntityTime, currentEntityTime, currentEntityTime, currentEntityTime });

            }
        }
    }

    //Aimbot functions
    if (settings[1] == 1){

        //If target is down or dead do not aimbot; return
        if (target->isBleedOut() || !target->isOkLifeState()) {
            Unprotect(_ReturnAddress());
            return;
        }

        //If friendly aimbot is on, only aimbot teammates (don't aim at enemies)
        if (settings[2] == 1) {
            if (entity_team != LPlayer->getTeamId()) {
                Unprotect(_ReturnAddress());
                return;
            }
        }

        //Otherwise aimbot enemies (don't aim at teammates)
        else {
            if (entity_team == LPlayer->getTeamId()) {
                Unprotect(_ReturnAddress());
                return;
            }
        }

        //Start doing aimbot math
        Vector BreathAngles = LPlayer->GetBreathAngles();
        Vector LocalCamera = LPlayer->GetCamPos();
        Vector Angle = Math::CalcAngle(LocalCamera, BonePosition);
        float fov = (float)Math::GetFov(BreathAngles, Angle, dist);
        if (fov < current_fov_limiter) {
            AimTarget = target->ptr;
            current_fov_limiter = fov;
        }
    }

    Unprotect(_ReturnAddress());
}

void UpdatePlayersInfo(Entity * LocalPlayer) {
    Protect(_ReturnAddress());

    current_fov_limiter = 999.f;
    AimTarget = 0;
    Spectators = 0;

    for (int i = 0; i <= 70; i++) {
        uintptr_t centity = Driver::read<uintptr_t>(GamePid, entitylist + ((uintptr_t)i << 5));
        if (centity == 0) continue;
        if (LocalPlayer->ptr == centity) continue;

        Unprotect(getEntity);
        Entity* Target = getEntity(GamePid, centity);
        Protect(getEntity);

        auto fptr = &Entity::isPlayer;
        Unprotect((void*)*(uintptr_t*)&fptr);
        if (!Target->isPlayer()) {
            Protect((void*)*(uintptr_t*)&fptr);
            delete Target;
            continue;
        }
        Protect((void*)*(uintptr_t*)&fptr);

        Unprotect(ProcessPlayer);
        ProcessPlayer(LocalPlayer, Target, entitylist, i);
        Protect(ProcessPlayer);
        vis_old[i] = Target->vis_time();
        delete Target;
    }

    Unprotect(_ReturnAddress());
}

void PredictPosition(Entity* LocalPlayer, Entity* target, Vector* BonePosition) {
    Protect(_ReturnAddress());
    uintptr_t current_weapon = LocalPlayer->CurrentWeapon(GamePid, entitylist);
    if (current_weapon != 0) { //if weapon found apply gravity & speed calculation
        float bulletSpeed = Driver::read<float>(GamePid, current_weapon + TOFFSET(OFFSET_BULLET_SPEED));
        float bulletGravity = Driver::read<float>(GamePid, current_weapon + TOFFSET(OFFSET_BULLET_GRAVITY));

        if (bulletSpeed > 1.0f) { //fix for charge rifle
            Vector muzzle = LocalPlayer->GetCamPos();
            float Time = BonePosition->DistTo(muzzle) / bulletSpeed;
            BonePosition->z += (700.f * bulletGravity * 0.5f) * (Time * Time);
            Vector velocity_delta = (target->GetVelocity() * Time);
            BonePosition->x += velocity_delta.x;
            BonePosition->y += velocity_delta.y;
            BonePosition->z += velocity_delta.z;
        }
    }
    Unprotect(_ReturnAddress());
}

uintptr_t nextBoneSwitch = 0;
uintptr_t StartTimeToAim = 0;
int CurrentTargetBone = 3;
int targets[] = { 7,5,3,2 };
int action = 1;
int boneIndex = 0;
Vector lastSet;
bool TargetLocked = false;

void AutoBoneSwitch() {
    Protect(_ReturnAddress());
    Unprotect(milliseconds_now);
    if (nextBoneSwitch < milliseconds_now()) {
        boneIndex += action;
        if (boneIndex == 3) {
            action = -1;
        }
        else if (boneIndex == 0) {
            action = 1;
        }
        CurrentTargetBone = targets[boneIndex];
        nextBoneSwitch = milliseconds_now() + 300;
    }
    Protect(milliseconds_now);
    Unprotect(_ReturnAddress());
}

void SmoothType_Asist(float fov, float TargetDistance, Vector* Delta, float smooth_multiplier) {
    Protect(_ReturnAddress());
    float smooth = 0.f;
    if (TargetDistance < 10.f) {
        smooth = 6.f + (smooth_multiplier - 1.f) * 3.f;
    }
    else {
        smooth = 6.f + (2.f + (smooth_multiplier - 1.f)) * fov;
    }
    if (smooth > 0.1f) {
        Delta->x /= smooth;
        Delta->y /= smooth;
        Delta->z /= smooth;
    }
    Unprotect(_ReturnAddress());
}

void SmoothType_TargetLock(float fov, float TargetDistance, Vector* Delta, float smooth_multiplier) {
    Protect(_ReturnAddress());
    if (!TargetLocked) {
        Unprotect(milliseconds_now);
        uintptr_t transcurrido = milliseconds_now() - StartTimeToAim;
        Protect(milliseconds_now);
        int max_time = (int)smooth_multiplier * 200;
        if (max_time > 1000) {
            max_time = 1000;
        }
        int restante = (int)(max_time - transcurrido);
        if (restante > 13) {
            float smooth = restante / 15.f;
            if (smooth > 1.f) {
                Delta->x /= smooth;
                Delta->y /= smooth;
                Delta->z /= smooth;
            }
        }
        else { //time passed
            TargetLocked = true;
        }
    }
    Unprotect(_ReturnAddress());
}

int AimAngles(Entity* LocalPlayer, Entity* target, Vector * out) {
    Protect(_ReturnAddress());
    Vector LocalCamera = LocalPlayer->GetCamPos();
    auto fptr = &Entity::getBonePosition;
    Unprotect((void*)*(uintptr_t*)&fptr);
    Vector BonePosition = target->getBonePosition(GamePid, CurrentTargetBone);
    Protect((void*)*(uintptr_t*)&fptr);
    Vector EntityPosition = target->getPosition();
    if (BonePosition.x == 0 || BonePosition.y == 0 || //check wrong player position and bone position
        LocalCamera.x == 0 || LocalCamera.y == 0 || //check wrong camera
        (BonePosition.x == EntityPosition.x && BonePosition.y == EntityPosition.y) //checks wrong bone position
        ) {
        Unprotect(_ReturnAddress());
        return 0;
    }
    Unprotect(PredictPosition);
    PredictPosition(LocalPlayer, target, &BonePosition);
    Protect(PredictPosition);

    Vector CalculatedAngles = Math::CalcAngle(LocalCamera, BonePosition);
    Vector ViewAngles = LocalPlayer->GetViewAngles();
    Vector DynBreath = LocalPlayer->GetBreathAngles();

    if (DynBreath.x == 0 || DynBreath.y == 0 || //Something was wrong
        ViewAngles.x == 0 || ViewAngles.y == 0) {
        Unprotect(_ReturnAddress());
        return 0;
    }
    Math::NormalizeAngles(DynBreath);

    Vector LocalPlayerPosition = LocalPlayer->getPosition();
    float TargetDistance = LocalPlayerPosition.DistTo(EntityPosition) / 39.62f;

    double fov = Math::GetFov(DynBreath, CalculatedAngles, TargetDistance); //fov based in distance to the target and angles (like create an sphere around the target, fov is the radius
    if (fov > 6.f || TargetDistance > Max_Cheat_Distance) {
        Unprotect(_ReturnAddress());
        return 0;
    }

    Vector Delta = CalculatedAngles - DynBreath;
    Math::NormalizeAngles(Delta);

    Unprotect(AutoBoneSwitch);
    AutoBoneSwitch();
    Protect(AutoBoneSwitch);

    Vector RecoilVec = LocalPlayer->GetRecoil();
    if (RecoilVec.x != 0 || RecoilVec.y != 0) {
        Delta -= (RecoilVec * 0.05f); //only a little as we are already fixing the recoil with breath angles
        Math::NormalizeAngles(Delta);
    }

    float fov2 = (float)Math::GetFov2(DynBreath, CalculatedAngles);
    if ((!(settings[3]==1)) || Spectators > 0 && settings[5] == 1) {
        Unprotect(SmoothType_Asist);
        SmoothType_Asist(fov2, TargetDistance , &Delta, SMOOTH);
        Protect(SmoothType_Asist);
    }
    else {
        Unprotect(SmoothType_TargetLock);
        SmoothType_TargetLock(fov2, TargetDistance, &Delta, SMOOTH);
        Protect(SmoothType_TargetLock);
    }

    Math::NormalizeAngles(Delta);


    Vector SmoothedAngles = ViewAngles + Delta;
    Math::NormalizeAngles(SmoothedAngles);
    if (lastSet == SmoothedAngles) {
        Unprotect(_ReturnAddress());
        return 2;
    }
    out->x = SmoothedAngles.x;
    out->y = SmoothedAngles.y;
    out->z = SmoothedAngles.z;
    Unprotect(_ReturnAddress());
    return 1;
}




//---------------------------------------------------------------------
//ATTEMPTING NO RECOIL
//---------------------------------------------------------------------

int RecoilDelete(Entity* LocalPlayer, Vector* ViewOut, Vector* PunchOut) {
    Protect(_ReturnAddress());
    //Get CurrentViewAngles
    Vector CurrentViewAngles = LocalPlayer->GetViewAngles();
    //Get the previous recoil angles
    Vector PrevPunch = { PunchOut->x, PunchOut->y, PunchOut->z };
    //Get new recoil angles
    Vector RecoilAngle = LocalPlayer->GetRecoil();
    //Create the new angle output vector
    Vector NewViewAngles = { 0.f, 0.f, 0.f };

    
    //Check for incorrect view angles
    if (CurrentViewAngles.x == 0 || CurrentViewAngles.y == 0) {
        Unprotect(_ReturnAddress());
        return 0;
    }

    //Adjust angles upon recoil
    if (RecoilAngle.x != 0.f || RecoilAngle.y != 0.f) {
        NewViewAngles = (CurrentViewAngles + PrevPunch) - RecoilAngle;
        Math::NormalizeAngles(NewViewAngles);
    }
    
    //Return 2 if no adjustment needed
    if (lastSet == NewViewAngles) {
        Unprotect(_ReturnAddress());
        return 2;
    }

    //Set new view angles
    ViewOut->x = NewViewAngles.x;
    ViewOut->y = NewViewAngles.y;
    ViewOut->z = NewViewAngles.z;

    LocalPlayer->SetViewAngles(GamePid, NewViewAngles);
    //Update previous punch angles
    PunchOut->x = RecoilAngle.x;
    PunchOut->y = RecoilAngle.y;
    PunchOut->z = RecoilAngle.z;
    Unprotect(_ReturnAddress());
    //Return 1 to adjust view
    return 1;
}
//---------------------------------------------------------------------
//ATTEMPTING NO RECOIL
//---------------------------------------------------------------------



void CheatLoop() {
    Protect(_ReturnAddress());
    entitylist = GameBaseAddress + TOFFSET(OFFSET_ENTITYLIST);
    uintptr_t lastAimTarget = 0;

    //Declare OldPunch outside the loop
    Vector OldPunch = { 0.f, 0.f, 0.f };
 
    while (true) {
        
        uintptr_t lptr = Driver::read<uintptr_t>(GamePid, GameBaseAddress + TOFFSET(OFFSET_LOCAL_ENT));
        if (lptr == 0) break;
        
        Unprotect(getEntity);
        Entity* LocalPlayer = getEntity(GamePid, lptr);
        Protect(getEntity);
        
        
        //(char*)(LocalPlayer->buffer + OFFSET_NAME)

        auto fptr = &Entity::isPlayer;
        Unprotect((void*)*(uintptr_t*)&fptr);
        if (!LocalPlayer->isPlayer()) {
            Protect((void*)*(uintptr_t*)&fptr);
            delete LocalPlayer;
            break;
        }
        Protect((void*)*(uintptr_t*)&fptr);

        Unprotect(milliseconds_now);
        if (nextEntityInfoUpdate < milliseconds_now()) {
            Protect(milliseconds_now);
            
            Unprotect(UpdatePlayersInfo);
            UpdatePlayersInfo(LocalPlayer);
            Protect(UpdatePlayersInfo);

        if (Spectators > 0 && printableOut) {
            char sp_str[] = { 'S','p','e','c','t','a','t','o','r','s',':',' ','%','l','l','u','\0' };
            printf(sp_str, Spectators);
            memset(sp_str, 0, sizeof(sp_str));
        }

        Unprotect(milliseconds_now);
        nextEntityInfoUpdate = milliseconds_now() + 100; //update info every 100ms
        Protect(milliseconds_now);
        }

        //First thing to do is check whether cheat is toggled on or off
        //Disable button is F11, turns off all cheats during gameplay if needed
        bool disable_pressed = (GetKeyState(VK_F11) & 0x8000);
        bool enable_pressed = (GetKeyState(VK_F12) & 0x8000);

        //Disable
        if (disable_pressed && toggle_set == 1) {
            settings[0] = 0;
            //settings[1] = 0;
            //settings[2] = 0;
            //settings[3] = 0;
            //settings[4] = 0;
            //settings[5] = 0;
            //settings[6] = 0;
            //settings[7] = 0;

            //Beeps confirm cheat is disable
            Beep(900, 100);
            Beep(800, 100);
            Beep(700, 100);

            //Set toggle button to 0 - cheats disabled
            toggle_set = 0;
        }

        //Re-enable
        if (enable_pressed && toggle_set == 0) {
            if (choice == 1) {
                settings[0] = 1;
                //settings[1] = 1;
                //settings[2] = 0;
                //settings[3] = 0;
                //settings[4] = 1;
                //settings[5] = 1;
                //settings[6] = 1;
                //settings[7] = 1;
            }

            if (choice == 2) {
                settings[0] = 1;
                //settings[1] = 1;
                //settings[2] = 1;
                //settings[3] = 1;
                //settings[4] = 0;
                //settings[5] = 0;
                //settings[6] = 0;
                //settings[7] = 1;
            }

            if (choice == 3) {
                settings[0] = 1;
                //settings[1] = 1;
                //settings[2] = 0;
                //settings[3] = 1;
                //settings[4] = 1;
                //settings[5] = 1;
                //settings[6] = 1;
                //settings[7] = 1;
            }

            //Beeps confirm cheat is enabled
            Beep(700, 100);
            Beep(800, 100);
            Beep(900, 100);

            //Set toggle button to 1 - cheats re-enabled
            toggle_set = 1;
        }

        //Get controller input

        //XINPUT_KEYSTROKE xStroke;
        /*if (XInputGetKeystroke(0, 0, &xStroke) == ERROR_SUCCESS) {
            std::cout << "Keystroke detected!" << std::endl;
            std::cout << "Virtual Key: " << xStroke.VirtualKey << std::endl;
            std::cout << "Flag: " << xStroke.Flags << std::endl;
        }

        bool left_trigger_pressed = (XInputGetKeystroke(0, 0, &xStroke) == ERROR_SUCCESS && xStroke.VirtualKey == 22534);
        bool right_trigger_pressed = (XInputGetKeystroke(0, 0, &xStroke) == ERROR_SUCCESS && xStroke.VirtualKey == 22535);

        if (left_trigger_pressed) {
            std::cout << "Left Trigger pressed!" << std::endl;
            std::cout << "Virtual Key: " << xStroke.VirtualKey << std::endl;
            std::cout << "Flag: " << xStroke.Flags << std::endl;
        }

        if (right_trigger_pressed) {
            std::cout << "Right Trigger pressed!" << std::endl;
            std::cout << "Virtual Key: " << xStroke.VirtualKey << std::endl;
            std::cout << "Flag: " << xStroke.Flags << std::endl;
        }*/


        //-----------------------------------------------------------------
        //Call no recoil function
        //-----------------------------------------------------------------

        if (settings[0] == 1) {
            Vector result1 = { 0.f,0.f,0.f };

            //Get CurrentViewAngles
            Vector CurrentViewAngles = LocalPlayer->GetViewAngles();
            //Get the previous recoil angles
            Vector PrevPunch = { OldPunch.x, OldPunch.y, OldPunch.z };
            //Get new recoil angles
            Vector RecoilAngle = LocalPlayer->GetRecoil();
            //Create the new angle output vector
            Vector NewViewAngles = { 0.f, 0.f, 0.f };

            //Adjust angles upon recoil
            if (RecoilAngle.x != 0.f || RecoilAngle.y != 0.f) {
                NewViewAngles = (CurrentViewAngles + PrevPunch) - RecoilAngle;
                Math::NormalizeAngles(NewViewAngles);
            }

            //Set new view angles if adjustment is needed
            if (lastSet != NewViewAngles) {

                LocalPlayer->SetViewAngles(GamePid, NewViewAngles);

                //Update previous punch angles
                OldPunch.x = RecoilAngle.x;
                OldPunch.y = RecoilAngle.y;
                OldPunch.z = RecoilAngle.z;
            }
        }

        if (settings[1] == 1) {
            Unprotect(milliseconds_now);
            bool aim_key_pressed = (GetKeyState(VK_RBUTTON) & 0x8000);
            if (AimTarget > 0 && aim_key_pressed && nextAim < milliseconds_now() && (Spectators > 0 && !(settings[4]==1) || Spectators == 0)) {
                Protect(milliseconds_now);

                if (lastAimTarget != AimTarget) {
                    TargetLocked = false;
                    Unprotect(milliseconds_now);
                    StartTimeToAim = milliseconds_now();
                    Protect(milliseconds_now);
                    lastAimTarget = AimTarget;
                }
                Unprotect(getEntity);
                Entity* target = getEntity(GamePid, AimTarget);
                Protect(getEntity);

                Vector result2 = { 0.f,0.f,0.f };

                Unprotect(AimAngles);
                int status = AimAngles(LocalPlayer, target, &result2);
                Protect(AimAngles);
                if (status == 1) { // 1 = movement needed, 2 = view already there, 0 = some out of aimbot params
                    LocalPlayer->SetViewAngles(GamePid, result2);
                }
                else if (status == 0) {
                    TargetLocked = false;
                    Unprotect(milliseconds_now);
                    StartTimeToAim = milliseconds_now();
                    Protect(milliseconds_now);
                }

                delete target;
                Unprotect(milliseconds_now);
                nextAim = milliseconds_now() + 16; //60 movements per second
                Protect(milliseconds_now);
            }
            else if (!aim_key_pressed || AimTarget == 0) {

                TargetLocked = false;
                Unprotect(milliseconds_now);
                StartTimeToAim = milliseconds_now();
                Protect(milliseconds_now);

                ProtectedSleep(1);
            }
        }

        else {
            ProtectedSleep(100);
        }
        Protect(milliseconds_now);
        
        delete LocalPlayer;
    }
    Unprotect(_ReturnAddress());
}

void Configure() {

    HWND consoleWnd = GetConsoleWindow();
    if (consoleWnd == NULL)
    {
        AllocConsole();
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
    else {
        printableOut = true;
    }

    char hi_str[] = { 'H','i',',',' ','W','e','l','c','o','m','e',' ','t','o',' ','C','R','Z',' ','E','F','I',' ','C','h','e','a','t','\n','A','n','s','w','e','r',' ','a','l','l',' ','t','h','e',' ','q','u','e','s','t','i','o','n','s','\n','\0' };
    std::cout << hi_str;
    memset(hi_str, 0, sizeof(hi_str));
    char hi2_str[] = { '1',' ','=',' ','P','u','b','l','i','c',' ','S','a','f','e',' ','M','o','d','e','\n','2',' ','=',' ','1','v','1',' ','M','o','d','e','\n','3',' ','=',' ','P','u','b','l','i','c',' ','A','i','m','l','o','c','k',' ','(','U','N','S','A','F','E',')','\n','\0' };
    std::cout << hi2_str;
    memset(hi2_str, 0, sizeof(hi2_str));
    char poop_str[] = { 'M','o','d','i','f','i','e','d',' ','b','y',' ','C','a','m','I','s','M','a','c','h','o','\n','\0' };
    std::cout << poop_str;
    memset(poop_str, 0, sizeof(poop_str));
    
    //Controller Check
    char ctrlr_cnct[] = { 'C', 'o', 'n', 't', 'r', 'o', 'l', 'l', 'e', 'r', ' ', 'C', 'o', 'n', 'n', 'e', 'c', 't', 'e', 'd', '!', '\n', '\0' };
    char ctrlr_dcnct[] = { 'C', 'o', 'n', 't', 'r', 'o', 'l', 'l', 'e', 'r', ' ', 'N', 'o', 't', ' ', 'C', 'o', 'n', 'n', 'e', 'c', 't', 'e', 'd', '!', '\n', '\0' };
    int controllerId = -1;

    for (DWORD i = 0; i < XUSER_MAX_COUNT && controllerId == -1; i++) {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        if (XInputGetState(i, &state) == ERROR_SUCCESS) {
            controllerId = i;
            std::cout << ctrlr_cnct;
            memset(ctrlr_cnct, 0, sizeof(ctrlr_cnct));
        }
    }

    if (controllerId == -1) {
        std::cout << ctrlr_dcnct;
        memset(ctrlr_dcnct, 0, sizeof(ctrlr_dcnct));
    }

    char choice_str[] = { '\n','M','a','k','e',' ','y','o','u','r',' ','s','e','l','e','c','t','i','o','n',':','\0' };
    std::cout << choice_str;
    memset(choice_str, 0, sizeof(choice_str));
    std::cin >> choice;
    std::cin.ignore();
    std::cin.clear();

    if (choice == 1) {
        settings[0] = 1;
        settings[1] = 1;
        settings[2] = 0;
        settings[3] = 0;
        settings[4] = 1;
        settings[5] = 1;
        settings[6] = 1;
        settings[7] = 1;
    }

    if (choice == 2) {
        settings[0] = 1;
        settings[1] = 1;
        settings[2] = 1;
        settings[3] = 1;
        settings[4] = 0;
        settings[5] = 0;
        settings[6] = 0;
        settings[7] = 1;
    }

    if (choice == 3) {
        settings[0] = 1;
        settings[1] = 1;
        settings[2] = 0;
        settings[3] = 1;
        settings[4] = 1;
        settings[5] = 1;
        settings[6] = 1;
        settings[7] = 1;
    }

    /*
    //Ask to enable no recoil
    char recoil_str[] = { '\n','D','o',' ','y','o','u',' ','w','a','n','t',' ','t','o',' ','e','n','a','b','l','e',' ','n','o',' ','r','e','c','o','i','l','?',':',' ','\0' };
    std::cout << recoil_str;
    memset(recoil_str, 0, sizeof(recoil_str));
    std::cin >> settings[0];
    og_enable_norecoil = enable_norecoil;
    std::cin.ignore();
    std::cin.clear();

    //Ask to enable normal aimbot
    char aimbot_str[] = { 'D','o',' ','y','o','u',' ','w','a','n','t',' ','t','o',' ','e','n','a','b','l','e',' ','a','i','m','b','o','t','?',':',' ','\0' };
    std::cout << aimbot_str;
    memset(aimbot_str, 0, sizeof(aimbot_str));
    std::cin >> enable_aimbot;
    og_enable_aimbot = enable_aimbot;
    std::cin.ignore();
    std::cin.clear();

    //Aimbot customization 
    if (enable_aimbot != 0) {
        //Ask to enable friendly aimbot
        char friendly_aimbot_str[] = { 'D','o',' ','y','o','u',' ','w','a','n','t',' ','t','o',' ','e','n','a','b','l','e',' ','f','r','i','e','n','d','l','y',' ','a','i','m','b','o','t','?',':',' ','\0' };
        std::cout << friendly_aimbot_str;
        memset(friendly_aimbot_str, 0, sizeof(friendly_aimbot_str));
        std::cin >> enable_friendly_aimbot;
        og_enable_friendly_aimbot = enable_friendly_aimbot;
        std::cin.ignore();
        std::cin.clear();

        //Ask to enable lock mode
        char aimbot_lock_str[] = { 'D','o',' ','y','o','u',' ','w','a','n','t',' ','t','o',' ','e','n','a','b','l','e',' ','a','i','m','b','o','t',' ','l','o','c','k',' ','m','o','d','e','?',' ','(','U','N','S','A','F','E',')',':',' ','\0' };
        std::cout << aimbot_lock_str;
        memset(aimbot_lock_str, 0, sizeof(aimbot_lock_str));
        std::cin >> enable_aimbot_lock_mode;
        og_enable_aimbot_lock_mode = enable_aimbot_lock_mode;
        //Always disable aim lock with spectators if it's on
        disable_aimbot_lock_mode_with_spectators = enable_aimbot_lock_mode;
        og_disable_aimbot_lock_mode_with_spectators = enable_aimbot_lock_mode;
        std::cin.ignore();
        std::cin.clear();

        //Ask to disable aimbot with spectators
        char disable_aimbot_str[] = { 'D','i','s','a','b','l','e',' ','a','i','m','b','o','t',' ','w','i','t','h',' ','s','p','e','c','t','a','t','o','r','s','?',':',' ','\0' };
        std::cout << disable_aimbot_str;
        memset(disable_aimbot_str, 0, sizeof(disable_aimbot_str));
        std::cin >> disable_aimbot_with_spectators;
        og_disable_aimbot_with_spectators = disable_aimbot_with_spectators;
        std::cin.ignore();
        std::cin.clear();

        //Ask to count team members as spectators
        //Disables aimbot when your team specatates
        char team_player_as_spectators_str[] = { 'C','o','u','n','t',' ','t','e','a','m',' ','p','l','a','y','e','r','s',' ','a','s',' ','s','p','e','c','t','a','t','o','r','s','?',':',' ','\0' };
        std::cout << team_player_as_spectators_str;
        memset(team_player_as_spectators_str, 0, sizeof(team_player_as_spectators_str));
        std::cin >> count_team_entities_as_spectators;
        og_count_team_entities_as_spectators = count_team_entities_as_spectators;
        std::cin.ignore();
        std::cin.clear();
    }

    //Ask to enable glow hack
    char enable_glow_str[] = { 'D','o',' ','y','o','u',' ','w','a','n','t',' ','t','o',' ','e','n','a','b','l','e',' ','g','l','o','w',' ','h','a','c','k','?',':',' ','\0' };
    std::cout << enable_glow_str;
    memset(enable_glow_str, 0, sizeof(enable_glow_str));
    std::cin >> enable_glow_hack;
    og_enable_glow_hack = enable_glow_hack;
    std::cin.ignore();
    std::cin.clear();
    */
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    system("cls");
   
    if (consoleWnd == NULL) {
        fclose(stdin);
        fclose(stdout);
        fclose(stderr);
        FreeConsole();
    }

}

DWORD WINAPI mainThread(PVOID) {
    SLog(L"Protecting");
    LoadProtectedFunctions();
    Protect(LoadProtectedFunctions);
    SLog(L"Connecting driver");
    Unprotect(Driver::initialize);
    Unprotect(CheckDriverStatus);
    if (!Driver::initialize() || !CheckDriverStatus()) {
        wchar_t VarName[] = { 'F','a','s','t','B','o','o','t','O','p','t','i','o','n','\0' };
        UNICODE_STRING FVariableName = UNICODE_STRING();
        FVariableName.Buffer = VarName;
        FVariableName.Length = 28;
        FVariableName.MaximumLength = 30;
        //UNICODE_STRING VariableName = RTL_CONSTANT_STRING(VARIABLE_NAME);
        myNtSetSystemEnvironmentValueEx(
            &FVariableName,
            &DummyGuid,
            0,
            0,
            ATTRIBUTES);//delete var
        memset(VarName, 0, sizeof(VarName));
        //memset(VariableName.Buffer, 0, VariableName.Length);
        //VariableName.Length = 0;
        Beep(600, 1000);
        char tx[] = { 'N','O',' ','E','F','I',' ',';','(','\n', 0 };
        printf(tx);
        ProtectedSleep(3000);
        exit(1);
        return 1;
    }
    Protect(Driver::initialize);
    Protect(CheckDriverStatus);

    Beep(900, 200);
    Beep(1100, 200);
    Beep(1300, 200);
    Beep(1500, 300);
    Beep(1700, 400);
    Beep(1900, 500);

    Unprotect(Configure);
    Configure();
    Protect(Configure);

    while (true) {
        wchar_t name[] = { 'r', '5', 'a', 'p', 'e', 'x', '.', 'e', 'x', 'e', 0 };
        //wchar_t name[] = { 'E', 'a', 's', 'y', 'A', 'n', 't', 'i', 'C', 'h', 'e', 'a', 't', '_', 'l', 'a', 'u', 'n', 'c', 'h', 'e', 'r', '.', 'e', 'x', 'e', 0 };
        Unprotect(GetProcessIdByName);
        DWORD pid = GetProcessIdByName(name);
        Protect(GetProcessIdByName);
        memset(name, 0, sizeof(name));

        Unprotect(Driver::GetBaseAddress);
        uintptr_t BaseAddr = Driver::GetBaseAddress(pid);
        Protect(Driver::GetBaseAddress);
       
        if (BaseAddr != 0) {
            GamePid = pid;
            GameBaseAddress = BaseAddr;
            
            Unprotect(CheatLoop);
            CheatLoop();
            Protect(CheatLoop);

            GamePid = 0;
            GameBaseAddress = 0;
        }
        ProtectedSleep(2000);
    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        SLog(L"Attach");
        HANDLE thread = CreateThread(NULL, NULL, mainThread, NULL, NULL, NULL);
        if (thread) 
            CloseHandle(thread);
    }
    return TRUE;
}

