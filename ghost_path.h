#if !defined(GHOST_PATH_H)

bool32 EntityCheck(v2 EyePos,
                   real32 EyeWidth,
                   real32 EyeDepth,
                   uint32 EyeDirection,
                   v2 EntityPos,
                   v2 EntityRadius,
                   uint32 *Tile)
{    
    //TODO: For pickupable objects, just check the EntityPos, no corners! yo
    //TODO: Have the two lines that determine the npc eyes start at the corners of the npc's face, not the center of his face!

    real32 NearSide = 0;
    real32 FarSide = 0;
    real32 LeftSide = 0;
    real32 RightSide = 0;
    //bool32 NS = false;
    //bool32 EW = false;
       
    if(EyeDirection == N)
    {

        NearSide = EntityPos.Y - EntityRadius.Y;
        FarSide = EntityPos.Y + EntityRadius.Y;
        LeftSide = EntityPos.X - EntityRadius.X;
        RightSide = EntityPos.X + EntityRadius.X;
        
        if((AbsDiff(NearSide,EyePos.Y) > EyeDepth) ||
           (FarSide < EyePos.Y) ||
           (AbsDiff(EntityPos.X,EyePos.X) > ((EyeWidth/2) + EntityRadius.X)))
        {
            return(false);
        }

        v2 Corner[4] =
            {
                {LeftSide, NearSide},
                {RightSide, NearSide},
                {LeftSide, FarSide},
                {RightSide, FarSide}
            };

        real32 NoseToEye = 1.0f;
        uint32 CornerAmount = 0;
        real32 LeftSlope = -EyeDepth/(EyeWidth/2);
        real32 RightSlope = EyeDepth/(EyeWidth/2);
        real32 LeftB = EyePos.Y - (LeftSlope*(EyePos.X - NoseToEye));
        real32 RightB = EyePos.Y - (RightSlope*(EyePos.X + NoseToEye));
        bool32 FarCorner = false;
        bool32 NearCorner = false;
        
        for(uint32 CI = 0; CI < 4; ++CI)
        {
            if((Corner[CI].Y > EyePos.Y) &&
               ((Corner[CI].Y - EyePos.Y) < EyeDepth))
            {
                real32 LeftXAtCornerY = (Corner[CI].Y - LeftB)/LeftSlope;
                real32 RightXAtCornerY = (Corner[CI].Y - RightB)/RightSlope;

                if((Corner[CI].X > LeftXAtCornerY) && (Corner[CI].X < RightXAtCornerY))
                {
                    ++CornerAmount;
                    if(Corner[CI].Y > NearSide + 0.1f)
                    {
                        FarCorner = true;
                    }
                    else
                    {
                        NearCorner = true;
                    }
                }
            }                
        }

        if(!CornerAmount)
        {
            return(false);
        }
        
        if(AbsDiff(NearSide, EyePos.Y) < 0.99f &&
           (Tile[((uint32)EyePos.Y * 45) + (uint32)EyePos.X] != WALL_TILE) &&
           (Tile[((uint32)NearSide * 45) + (uint32)EntityPos.X] != WALL_TILE))
        {
            return(true);
        }
                

        bool32 ToTheLeft = (EntityPos.X < EyePos.X);
        v2 Eye[3] =
        {
            {EyePos.X - NoseToEye, EyePos.Y},
            {EyePos.X, EyePos.Y},
            {EyePos.X + NoseToEye, EyePos.Y}
        };
        
        v2 Point[3] = {};
        
        switch(CornerAmount)
        {
            
            case 1:
            {
                if(FarCorner)
                {
                    if(NearSide <= EyePos.Y + 0.5f)
                    {
                        return(true);
                    }
                    
                    if(ToTheLeft)
                    {
                        Point[0].X = RightSide;
                        Point[0].Y = RightSide*LeftSlope + LeftB;
                        Point[1] = Corner[3];
                        Point[2] = Corner[3];
                    }
                    else
                    {
                        Point[0] = Corner[2];
                        Point[1] = Corner[2];
                        Point[2].X = LeftSide;
                        Point[2].Y = LeftSide*RightSlope + RightB;
                    }
                }
                else
                {
                    if(ToTheLeft)
                    {
                        Point[0].X = (NearSide - LeftB)/LeftSlope;
                        Point[0].Y = NearSide;
                        Point[1].X = RightSide;
                        Point[1].Y = NearSide;
                        Point[2].X = RightSide;
                        Point[2].Y = EyePos.Y + EyeDepth;
                    }
                    else
                    {
                        Point[0].X = LeftSide;
                        Point[0].Y = EyePos.Y + EyeDepth;
                        Point[1].X = LeftSide;
                        Point[1].Y = NearSide;
                        Point[2].X = (NearSide - RightB)/RightSlope;
                        Point[2].Y = NearSide;
                    }
                }                
            }break;

            case 2:
            case 3:   
            {
                if(!NearCorner)
                {                    
                    return(true);
                }
                else if(!FarCorner)
                {                        
                    Point[0] = Corner[0];
                    Point[1].X = EntityPos.X;
                    Point[1].Y = NearSide;
                    Point[2] = Corner[1];                        
                }
                else
                {
                    if(ToTheLeft)
                    {
                        Point[0].X = (NearSide - LeftB)/LeftSlope;
                        Point[0].Y = NearSide;
                        Point[1] = Corner[1];
                        Point[2] = Corner[3];
                    }
                    else
                    {
                        Point[0] = Corner[2];
                        Point[1] = Corner[0];
                        Point[2].X = (NearSide - RightB)/RightSlope;
                        Point[2].Y = NearSide;
                    }
                }
            }break;            
            case 4:
            {
                
                if(ToTheLeft)
                {
                    Point[0] = Corner[0];
                    if(RightSide < EntityPos.X)
                    {
                        Point[1] = Corner[1];
                        Point[2] = Corner[3];                        
                    }
                    else
                    {
                        Point[1].X = EntityPos.X;
                        Point[1].Y = NearSide;
                        Point[2] = Corner[1];
                    }
                            
                }
                else
                {
                    Point[2] = Corner[1];
                    if(LeftSide > EntityPos.X)
                    {
                        Point[0] = Corner[2];
                        Point[1] = Corner[0];                        
                    }
                    else
                    {
                        Point[0] = Corner[0];
                        Point[1] = v2{EntityPos.X, NearSide};
                    }
                }
            }break;
        };

        bool32 EyeObstructed[3] = {};
        uint32 TileID = 0;
        real32 XAtY = 0;
        real32 EyeSlope = 0;
        real32 EyeB = 0;
        
        for(uint32 EyeI = 0; EyeI < 3; ++EyeI)
        {
            EyeSlope = (Point[EyeI].Y - Eye[EyeI].Y)/(Point[EyeI].X - Eye[EyeI].X);
            EyeB = Eye[EyeI].Y - EyeSlope*Eye[EyeI].X;
            
            for(uint32 UnitStep = (uint32)Eye[EyeI].Y; UnitStep < FarSide; ++UnitStep)
            {

                //TODO: This was causing XAtY to be some large negative number because of the slope being so high.
                //Possibly fix by saying that if slope is > 100(or some appropriate number, just make XAtY be equal to Point[EyeI].X
                
                XAtY = (UnitStep - EyeB)/EyeSlope;

                if(XAtY < 0 && XAtY > 45)
                {
                    //TODO:
                    continue;
                }
                
                TileID = Tile[(UnitStep * 45) + (uint32)XAtY];
                
                if(TileID == WALL_TILE || TileID == OUTER_WALL_TILE)
                {
                    EyeObstructed[EyeI] = true;
                    break;
                }
            }
            
            if(!EyeObstructed[EyeI])
            {
                return(true);
            }
        }

        return(false);                
    }     
    else if(EyeDirection == S)
    {
            NearSide = EntityPos.Y + EntityRadius.Y;
            FarSide = EntityPos.Y - EntityRadius.Y;
            LeftSide = EntityPos.X + EntityRadius.X;
            RightSide = EntityPos.X - EntityRadius.X;
            
        if((AbsDiff(NearSide,EyePos.Y) > EyeDepth) ||
           //NOTE: S
           (FarSide > EyePos.Y) ||
           (AbsDiff(EntityPos.X,EyePos.X) > ((EyeWidth/2) + EntityRadius.X)))
        {
            return(false);
        }

        v2 Corner[4] =
            {
                {LeftSide, NearSide},
                {RightSide, NearSide},
                {LeftSide, FarSide},
                {RightSide, FarSide}
            };

        real32 NoseToEye = 1.0f;
        uint32 CornerAmount = 0;
        real32 LeftSlope = -EyeDepth/(EyeWidth/2);
        real32 RightSlope = EyeDepth/(EyeWidth/2);
        real32 LeftB = EyePos.Y - (LeftSlope*(EyePos.X + NoseToEye));
        real32 RightB = EyePos.Y - (RightSlope*(EyePos.X - NoseToEye));
        bool32 FarCorner = false;
        bool32 NearCorner = false;
        
        for(uint32 CI = 0; CI < 4; ++CI)
        {
            if((Corner[CI].Y < EyePos.Y) &&
               ((EyePos.Y - Corner[CI].Y) < EyeDepth))
            {
                real32 LeftXAtCornerY = (Corner[CI].Y - LeftB)/LeftSlope;
                real32 RightXAtCornerY = (Corner[CI].Y - RightB)/RightSlope;

                if((Corner[CI].X < LeftXAtCornerY) && (Corner[CI].X > RightXAtCornerY))
                {
                    ++CornerAmount;
                    if(Corner[CI].Y < NearSide - 0.1f)
                    {
                        FarCorner = true;
                    }
                    else
                    {
                        NearCorner = true;
                    }
                }
            }                
        }

        if(!CornerAmount)
        {
            return(false);
        }
        
        if(AbsDiff(NearSide, EyePos.Y) < 0.99f &&
           (Tile[((uint32)EyePos.Y * 45) + (uint32)EyePos.X] != WALL_TILE) &&
           (Tile[((uint32)NearSide * 45) + (uint32)EntityPos.X] != WALL_TILE))
        {
            return(true);
        }
                

        bool32 ToTheLeft = (EntityPos.X > EyePos.X);
        v2 Eye[3] =
            {
                {EyePos.X + NoseToEye, EyePos.Y},
                {EyePos.X, EyePos.Y},
                {EyePos.X - NoseToEye, EyePos.Y}
            };
        
        v2 Point[3] = {};
        
        switch(CornerAmount)
        {
            
            case 1:
            {
                if(FarCorner)
                {
                    if(NearSide >= EyePos.Y + 0.5f)
                    {
                        return(true);
                    }
                    
                    if(ToTheLeft)
                    {
                        Point[0].X = RightSide;
                        Point[0].Y = RightSide*LeftSlope + LeftB;
                        Point[1] = Corner[3];
                        Point[2] = Corner[3];
                    }
                    else
                    {
                        Point[0] = Corner[2];
                        Point[1] = Corner[2];
                        Point[2].X = LeftSide;
                        Point[2].Y = LeftSide*RightSlope + RightB;
                    }
                }
                else
                {
                    if(ToTheLeft)
                    {
                        Point[0].X = (NearSide - LeftB)/LeftSlope;
                        Point[0].Y = NearSide;
                        Point[1].X = RightSide;
                        Point[1].Y = NearSide;
                        Point[2].X = RightSide;
                        Point[2].Y = EyePos.Y - EyeDepth;
                    }
                    else
                    {
                        Point[0].X = LeftSide;
                        Point[0].Y = EyePos.Y - EyeDepth;
                        Point[1].X = LeftSide;
                        Point[1].Y = NearSide;
                        Point[2].X = (NearSide - RightB)/RightSlope;
                        Point[2].Y = NearSide;
                    }
                }                
            }break;

            case 2:
            case 3:   
            {
                if(!NearCorner)
                {                    
                    return(true);
                }
                else if(!FarCorner)
                {                        
                    Point[0] = Corner[0];
                    Point[1].X = EntityPos.X;
                    Point[1].Y = NearSide;
                    Point[2] = Corner[1];                        
                }
                else
                {
                    if(ToTheLeft)
                    {
                        Point[0].X = (NearSide - LeftB)/LeftSlope;
                        Point[0].Y = NearSide;
                        Point[1] = Corner[1];
                        Point[2] = Corner[3];
                    }
                    else
                    {
                        Point[0] = Corner[2];
                        Point[1] = Corner[0];
                        Point[2].X = (NearSide - RightB)/RightSlope;
                        Point[2].Y = NearSide;
                    }
                }
            }break;            
            case 4:
            {
                if(ToTheLeft)
                {
                    Point[0] = Corner[0];
                    if(RightSide > EntityPos.X)
                    {
                        Point[1] = Corner[1];
                        Point[2] = Corner[3];                        
                    }
                    else
                    {
                        Point[1].X = EntityPos.X;
                        Point[1].Y = NearSide;
                        Point[2] = Corner[1];
                    }
                            
                }
                else
                {
                    Point[2] = Corner[1];
                    if(LeftSide < EntityPos.X)
                    {
                        Point[0] = Corner[2];
                        Point[1] = Corner[0];                        
                    }
                    else
                    {
                        Point[0] = Corner[0];
                        Point[1] = v2{EntityPos.X, NearSide};
                    }
                }
            }break;
        };

        bool32 EyeObstructed[3] = {};
        uint32 TileID = 0;
        real32 XAtY = 0;
        real32 EyeSlope = 0;
        real32 EyeB = 0;
        
        for(uint32 EyeI = 0; EyeI < 3; ++EyeI)
        {
            EyeSlope = (Point[EyeI].Y - Eye[EyeI].Y)/(Point[EyeI].X - Eye[EyeI].X);
            EyeB = Eye[EyeI].Y - EyeSlope*Eye[EyeI].X;
            
            for(uint32 UnitStep = (uint32)Eye[EyeI].Y; UnitStep > (uint32)(FarSide-1); --UnitStep)
            {

                //TODO: This was causing XAtY to be some large negative number because of the slope being so high.
                //Possibly fix by saying that if slope is > 100(or some appropriate number, just make XAtY be equal to Point[EyeI].X
                
                XAtY = (UnitStep - EyeB)/EyeSlope;

                if(XAtY < 0 && XAtY > 45)
                {
                    //TODO
                    continue;
                }
                
                TileID = Tile[(UnitStep * 45) + (uint32)XAtY];
                
                if(TileID == WALL_TILE || TileID == OUTER_WALL_TILE)
                {
                    EyeObstructed[EyeI] = true;
                    break;
                }
            }
            
            if(!EyeObstructed[EyeI])
            {
                return(true);
            }
        }

        return(false);                
    }
    else if(EyeDirection == E)
    {
            NearSide = EntityPos.X - EntityRadius.X;
            FarSide = EntityPos.X + EntityRadius.X;
            LeftSide = EntityPos.Y + EntityRadius.Y;
            RightSide = EntityPos.Y - EntityRadius.Y;
            
        if((AbsDiff(NearSide,EyePos.X) > EyeDepth) ||
           (FarSide < EyePos.X) ||
           (AbsDiff(EntityPos.Y,EyePos.Y) > ((EyeWidth/2) + EntityRadius.Y)))
        {
            return(false);
        }

        v2 Corner[4] =
            {
                {NearSide, LeftSide},
                {NearSide, RightSide},
                {FarSide, LeftSide},
                {FarSide, RightSide}
            };

        real32 NoseToEye = 0.75f;
        uint32 CornerAmount = 0;
        real32 LeftSlope = (EyeWidth/2)/EyeDepth;
        real32 RightSlope = -(EyeWidth/2)/EyeDepth;
        real32 LeftB = (EyePos.Y + NoseToEye) - (LeftSlope*EyePos.X);
        real32 RightB = (EyePos.Y - NoseToEye) - (RightSlope*EyePos.X);
        bool32 FarCorner = false;
        bool32 NearCorner = false;
        
        for(uint32 CI = 0; CI < 4; ++CI)
        {
            if((Corner[CI].X > EyePos.X) &&
               ((Corner[CI].X - EyePos.X) < EyeDepth))
            {
                real32 LeftYAtCornerX = Corner[CI].X*LeftSlope + LeftB;
                real32 RightYAtCornerX = Corner[CI].X *RightSlope + RightB;

                if((Corner[CI].Y < LeftYAtCornerX) && (Corner[CI].Y > RightYAtCornerX))
                {
                    ++CornerAmount;
                    if(Corner[CI].X > NearSide + 0.01f)
                    {
                        FarCorner = true;
                    }
                    else
                    {
                        NearCorner = true;
                    }
                }
            }                
        }

        if(!CornerAmount)
        {
            return(false);
        }
        
        if(AbsDiff(NearSide, EyePos.X) < 0.99f &&
           (Tile[((uint32)EyePos.Y * 45) + (uint32)EyePos.X] != WALL_TILE) &&
           (Tile[((uint32)EntityPos.Y * 45) + (uint32)NearSide] != WALL_TILE))
        {
            return(true);
        }
                

        bool32 ToTheLeft = (EntityPos.Y > EyePos.Y);
        v2 Eye[3] =
            {
                {EyePos.X, EyePos.Y + NoseToEye},
                {EyePos.X, EyePos.Y},
                {EyePos.X, EyePos.Y - NoseToEye}
            };
        
        v2 Point[3] = {};
        
        switch(CornerAmount)
        {
            
            case 1:
            {
                if(FarCorner)
                {
                    if(NearSide <= EyePos.X + 0.5f)
                    {
                        return(true);
                    }
                    
                    if(ToTheLeft)
                    {
                        Point[0].Y = RightSide;
                        Point[0].X = (RightSide - LeftB)/LeftSlope;
                        Point[1] = Corner[3];
                        Point[2] = Corner[3];
                    }
                    else
                    {
                        Point[0] = Corner[2];
                        Point[1] = Corner[2];
                        Point[2].Y = LeftSide;
                        Point[2].X = (LeftSide - RightB)/RightSlope;
                    }
                }
                else
                {
                    if(ToTheLeft)
                    {
                        Point[0].Y = (NearSide*LeftSlope + LeftB);
                        Point[0].X = NearSide;
                        Point[1].Y = RightSide;
                        Point[1].X = NearSide;
                        Point[2].Y = RightSide;
                        Point[2].X = EyePos.X + EyeDepth;
                    }
                    else
                    {
                        Point[0].Y = LeftSide;
                        Point[0].X = EyePos.X + EyeDepth;
                        Point[1].Y = LeftSide;
                        Point[1].X = NearSide;
                        Point[2].Y = (NearSide*RightSlope + RightB);
                        Point[2].X = NearSide;
                    }
                }                
            }break;

            case 2:
            case 3:   
            {
                if(!NearCorner)
                {                    
                    return(true);
                }
                else if(!FarCorner)
                {                        
                    Point[0] = Corner[0];
                    Point[1].Y = EntityPos.Y;
                    Point[1].X = NearSide;
                    Point[2] = Corner[1];                        
                }
                else
                {
                    if(ToTheLeft)
                    {
                        Point[0].Y = NearSide*LeftSlope + LeftB;
                        Point[0].X = NearSide;
                        Point[1] = Corner[1];
                        Point[2] = Corner[3];
                    }
                    else
                    {
                        Point[0] = Corner[2];
                        Point[1] = Corner[0];
                        Point[2].Y = NearSide*RightSlope + RightB;
                        Point[2].X = NearSide;
                    }
                }
            }break;            
            case 4:
            {
                if(ToTheLeft)
                {
                    Point[0] = Corner[0];
                    if(RightSide > EntityPos.Y)
                    {
                        Point[1] = Corner[1];
                        Point[2] = Corner[3];                        
                    }
                    else
                    {
                        Point[1].Y = EntityPos.Y;
                        Point[1].X = NearSide;
                        Point[2] = Corner[1];
                    }
                            
                }
                else
                {
                    Point[2] = Corner[1];
                    if(LeftSide < EntityPos.Y)
                    {
                        Point[0] = Corner[2];
                        Point[1] = Corner[0];                        
                    }
                    else
                    {
                        Point[0] = Corner[0];
                        Point[1] = v2{NearSide, EntityPos.Y};
                    }
                }
            }break;
        };

        bool32 EyeObstructed[3] = {};
        uint32 TileID = 0;
        real32 YAtX = 0;
        real32 EyeSlope = 0;
        real32 EyeB = 0;
        
        for(uint32 EyeI = 0; EyeI < 3; ++EyeI)
        {
            EyeSlope = (Point[EyeI].Y - Eye[EyeI].Y)/(Point[EyeI].X - Eye[EyeI].X);
            EyeB = Eye[EyeI].Y - EyeSlope*Eye[EyeI].X;
            
            for(uint32 UnitStep = (uint32)Eye[EyeI].X; UnitStep < FarSide; ++UnitStep)
            {

                //TODO: This was causing XAtY to be some large negative number because of the slope being so high.
                //Possibly fix by saying that if slope is > 100(or some appropriate number, just make XAtY be equal to Point[EyeI].X
                
                YAtX = EyeSlope*UnitStep + EyeB;

                if(YAtX < 0 && YAtX > 50)
                {
                    //TODO
                    continue;
                }
                
                TileID = Tile[((uint32)YAtX * 45) + UnitStep];
                
                if(TileID == WALL_TILE || TileID == OUTER_WALL_TILE)
                {
                    EyeObstructed[EyeI] = true;
                    break;
                }
            }
            
            if(!EyeObstructed[EyeI])
            {
                return(true);
            }
        }

        return(false);                
    }
    else if(EyeDirection == W)
    {
            NearSide = EntityPos.X + EntityRadius.X;
            FarSide = EntityPos.X - EntityRadius.X;
            LeftSide = EntityPos.Y - EntityRadius.Y;
            RightSide = EntityPos.Y + EntityRadius.Y;
            
        if((AbsDiff(NearSide,EyePos.X) > EyeDepth) ||
           (FarSide > EyePos.X) ||
           (AbsDiff(EntityPos.Y,EyePos.Y) > ((EyeWidth/2) + EntityRadius.Y)))
        {
            return(false);
        }

        v2 Corner[4] =
            {
                {NearSide, LeftSide},
                {NearSide, RightSide},
                {FarSide, LeftSide},
                {FarSide, RightSide}
            };

        real32 NoseToEye = 0.75f;
        uint32 CornerAmount = 0;
        real32 LeftSlope = (EyeWidth/2)/EyeDepth;
        real32 RightSlope = -(EyeWidth/2)/EyeDepth;
        real32 LeftB = (EyePos.Y - NoseToEye) - (LeftSlope*EyePos.X);
        real32 RightB = (EyePos.Y + NoseToEye) - (RightSlope*EyePos.X);
        bool32 FarCorner = false;
        bool32 NearCorner = false;
        
        for(uint32 CI = 0; CI < 4; ++CI)
        {
            if((Corner[CI].X < EyePos.X) &&
               ((EyePos.X - Corner[CI].X) < EyeDepth))
            {
                real32 LeftYAtCornerX = Corner[CI].X*LeftSlope + LeftB;
                real32 RightYAtCornerX = Corner[CI].X *RightSlope + RightB;

                if((Corner[CI].Y > LeftYAtCornerX) && (Corner[CI].Y < RightYAtCornerX))
                {
                    ++CornerAmount;
                    if(Corner[CI].X > NearSide + 0.01f)
                    {
                        FarCorner = true;
                    }
                    else
                    {
                        NearCorner = true;
                    }
                }
            }                
        }

        if(!CornerAmount)
        {
            return(false);
        }
        
        if(AbsDiff(NearSide, EyePos.X) < 0.99f &&
           (Tile[((uint32)EyePos.Y * 45) + (uint32)EyePos.X] != WALL_TILE) &&
           (Tile[((uint32)EntityPos.Y * 45) + (uint32)NearSide] != WALL_TILE))
        {
            return(true);
        }
                

        bool32 ToTheLeft = (EntityPos.Y < EyePos.Y);
        v2 Eye[3] =
            {
                {EyePos.X, EyePos.Y - NoseToEye},
                {EyePos.X, EyePos.Y},
                {EyePos.X, EyePos.Y + NoseToEye}
            };
        
        v2 Point[3] = {};
        
        switch(CornerAmount)
        {
            
            case 1:
            {
                if(FarCorner)
                {
                    if(AbsDiff(NearSide, EyePos.X) < 0.5f)
                    {
                        return(true);
                    }
                    
                    if(ToTheLeft)
                    {
                        Point[0].Y = RightSide;
                        Point[0].X = (RightSide - LeftB)/LeftSlope;
                        Point[1] = Corner[3];
                        Point[2] = Corner[3];
                    }
                    else
                    {
                        Point[0] = Corner[2];
                        Point[1] = Corner[2];
                        Point[2].Y = LeftSide;
                        Point[2].X = (LeftSide - RightB)/RightSlope;
                    }
                }
                else
                {
                    if(ToTheLeft)
                    {
                        Point[0].Y = (NearSide*LeftSlope + LeftB);
                        Point[0].X = NearSide;
                        Point[1].Y = RightSide;
                        Point[1].X = NearSide;
                        Point[2].Y = RightSide;
                        Point[2].X = EyePos.X - EyeDepth;
                    }
                    else
                    {
                        Point[0].Y = LeftSide;
                        Point[0].X = EyePos.X - EyeDepth;
                        Point[1].Y = LeftSide;
                        Point[1].X = NearSide;
                        Point[2].Y = (NearSide*RightSlope + RightB);
                        Point[2].X = NearSide;
                    }
                }                
            }break;

            case 2:
            case 3:   
            {
                if(!NearCorner)
                {                    
                    return(true);
                }
                else if(!FarCorner)
                {                        
                    Point[0] = Corner[0];
                    Point[1].Y = EntityPos.Y;
                    Point[1].X = NearSide;
                    Point[2] = Corner[1];                        
                }
                else
                {
                    if(ToTheLeft)
                    {
                        Point[0].Y = NearSide*LeftSlope + LeftB;
                        Point[0].X = NearSide;
                        Point[1] = Corner[1];
                        Point[2] = Corner[3];
                    }
                    else
                    {
                        Point[0] = Corner[2];
                        Point[1] = Corner[0];
                        Point[2].Y = NearSide*RightSlope + RightB;
                        Point[2].X = NearSide;
                    }
                }
            }break;            
            case 4:
            {
                if(ToTheLeft)
                {
                    Point[0] = Corner[0];
                    if(RightSide < EntityPos.Y)
                    {
                        Point[1] = Corner[1];
                        Point[2] = Corner[3];                        
                    }
                    else
                    {
                        Point[1].Y = EntityPos.Y;
                        Point[1].X = NearSide;
                        Point[2] = Corner[1];
                    }
                            
                }
                else
                {
                    Point[2] = Corner[1];
                    if(LeftSide < EntityPos.Y)
                    {
                        Point[0] = Corner[2];
                        Point[1] = Corner[0];                        
                    }
                    else
                    {
                        Point[0] = Corner[0];
                        Point[1] = v2{NearSide, EntityPos.Y};
                    }
                }
            }break;
        };

        bool32 EyeObstructed[3] = {};
        uint32 TileID = 0;
        real32 YAtX = 0;
        real32 EyeSlope = 0;
        real32 EyeB = 0;
        
        for(uint32 EyeI = 0; EyeI < 3; ++EyeI)
        {
            EyeSlope = (Point[EyeI].Y - Eye[EyeI].Y)/(Point[EyeI].X - Eye[EyeI].X);
            EyeB = Eye[EyeI].Y - EyeSlope*Eye[EyeI].X;

                for(uint32 UnitStep = (uint32)Eye[EyeI].X; UnitStep > (uint32)(FarSide - 1); --UnitStep)
            {

                //TODO: This was causing XAtY to be some large negative number because of the slope being so high.
                //Possibly fix by saying that if slope is > 100(or some appropriate number, just make XAtY be equal to Point[EyeI].X
                
                YAtX = EyeSlope*UnitStep + EyeB;

                if(YAtX < 0 && YAtX > 50)
                {
                    //TODO
                    continue;
                }
                
                TileID = Tile[((uint32)YAtX * 45) + UnitStep];
                
                if(TileID == WALL_TILE || TileID == OUTER_WALL_TILE)
                {
                    EyeObstructed[EyeI] = true;
                    break;
                }
            }
            
            if(!EyeObstructed[EyeI])
            {
                return(true);
            }
        }

        return(false);                
    }        
    return(false);            
}

bool32 ObjCheck(v2 EyePos,
                   real32 EyeWidth,
                   real32 EyeDepth,
                   uint32 EyeDirection,
                   v2 EntityPos,          
                   uint32 *Tile)
{    
       
    if(EyeDirection == N)
    {
        
        if((AbsDiff(EntityPos.Y,EyePos.Y) > EyeDepth) ||
           (EntityPos.Y < EyePos.Y) ||
           (AbsDiff(EntityPos.X,EyePos.X) > ((EyeWidth/2))))
        {
            return(false);
        }

        real32 NoseToEye = 1.0f;

        real32 LeftSlope = -EyeDepth/(EyeWidth/2);
        real32 RightSlope = EyeDepth/(EyeWidth/2);
        real32 LeftB = EyePos.Y - (LeftSlope*(EyePos.X - NoseToEye));
        real32 RightB = EyePos.Y - (RightSlope*(EyePos.X + NoseToEye));

        bool32 ObjInRange = false;
        
            if((EntityPos.Y > EyePos.Y) &&
               ((EntityPos.Y - EyePos.Y) < EyeDepth))
            {
                real32 LeftXAtCornerY = (EntityPos.Y - LeftB)/LeftSlope;
                real32 RightXAtCornerY = (EntityPos.Y - RightB)/RightSlope;

                if((EntityPos.X > LeftXAtCornerY) && (EntityPos.X < RightXAtCornerY))
                {

                    ObjInRange = true;
                }
            }                


        if(!ObjInRange)
        {
            return(false);
        }
        
        if(AbsDiff(EntityPos.Y, EyePos.Y) < 0.99f &&
           (Tile[((uint32)EyePos.Y * 45) + (uint32)EyePos.X] != WALL_TILE) &&
           (Tile[((uint32)EntityPos.Y * 45) + (uint32)EntityPos.X] != WALL_TILE))
        {
            return(true);
        }                
        v2 Eye[3] =
        {
            {EyePos.X - NoseToEye, EyePos.Y},
            {EyePos.X, EyePos.Y},
            {EyePos.X + NoseToEye, EyePos.Y}
        };
        
        bool32 EyeObstructed[3] = {};
        uint32 TileID = 0;
        real32 XAtY = 0;
        real32 EyeSlope = 0;
        real32 EyeB = 0;
        
        for(uint32 EyeI = 0; EyeI < 3; ++EyeI)
        {
            EyeSlope = (EntityPos.Y - Eye[EyeI].Y)/(EntityPos.X - Eye[EyeI].X);
            EyeB = Eye[EyeI].Y - EyeSlope*Eye[EyeI].X;
            
            for(uint32 UnitStep = (uint32)Eye[EyeI].Y; UnitStep < EntityPos.Y; ++UnitStep)
            {                
                XAtY = (UnitStep - EyeB)/EyeSlope;

                if(XAtY < 0 && XAtY > 45)
                {
                    //TODO
                    continue;
                }
                
                TileID = Tile[(UnitStep * 45) + (uint32)XAtY];
                
                if(TileID == WALL_TILE || TileID == OUTER_WALL_TILE)
                {
                    EyeObstructed[EyeI] = true;
                    break;
                }
            }
            
            if(!EyeObstructed[EyeI])
            {
                return(true);
            }
        }

        return(false);                
    }     
    else if(EyeDirection == S)
    {            
        if((AbsDiff(EntityPos.Y,EyePos.Y) > EyeDepth) ||
           //NOTE: S
           (EntityPos.Y > EyePos.Y) ||
           (AbsDiff(EntityPos.X,EyePos.X) > ((EyeWidth/2))))
        {
            return(false);
        }

        real32 NoseToEye = 1.0f;

        real32 LeftSlope = -EyeDepth/(EyeWidth/2);
        real32 RightSlope = EyeDepth/(EyeWidth/2);
        real32 LeftB = EyePos.Y - (LeftSlope*(EyePos.X + NoseToEye));
        real32 RightB = EyePos.Y - (RightSlope*(EyePos.X - NoseToEye));
        uint32 CornerAmount = 0;

            if((EntityPos.Y < EyePos.Y) &&
               ((EyePos.Y - EntityPos.Y) < EyeDepth))
            {
                real32 LeftXAtCornerY = (EntityPos.Y - LeftB)/LeftSlope;
                real32 RightXAtCornerY = (EntityPos.Y - RightB)/RightSlope;

                if((EntityPos.X < LeftXAtCornerY) && (EntityPos.X > RightXAtCornerY))
                {
                    ++CornerAmount;                    
                }
            }                
        

        if(!CornerAmount)
        {
            return(false);
        }
        
        if(AbsDiff(EntityPos.Y, EyePos.Y) < 0.99f &&
           (Tile[((uint32)EyePos.Y * 45) + (uint32)EyePos.X] != WALL_TILE) &&
           (Tile[((uint32)EntityPos.Y * 45) + (uint32)EntityPos.X] != WALL_TILE))
        {
            return(true);
        }

        v2 Eye[3] =
            {
                {EyePos.X + NoseToEye, EyePos.Y},
                {EyePos.X, EyePos.Y},
                {EyePos.X - NoseToEye, EyePos.Y}
            };

        bool32 EyeObstructed[3] = {};
        uint32 TileID = 0;
        real32 XAtY = 0;
        real32 EyeSlope = 0;
        real32 EyeB = 0;
        
        for(uint32 EyeI = 0; EyeI < 3; ++EyeI)
        {
            EyeSlope = (EntityPos.Y - Eye[EyeI].Y)/(EntityPos.X - Eye[EyeI].X);
            EyeB = Eye[EyeI].Y - EyeSlope*Eye[EyeI].X;
            
            for(uint32 UnitStep = (uint32)Eye[EyeI].Y; UnitStep > (uint32)(EntityPos.Y-1); --UnitStep)
            {

                //TODO: This was causing XAtY to be some large negative number because of the slope being so high.
                //Possibly fix by saying that if slope is > 100(or some appropriate number, just make XAtY be equal to EntityPos.X
                
                XAtY = (UnitStep - EyeB)/EyeSlope;

                if(XAtY < 0 && XAtY > 45)
                {
                    //TODO
                    continue;
                }
                
                TileID = Tile[(UnitStep * 45) + (uint32)XAtY];
                
                if(TileID == WALL_TILE || TileID == OUTER_WALL_TILE)
                {
                    EyeObstructed[EyeI] = true;
                    break;
                }
            }
            
            if(!EyeObstructed[EyeI])
            {
                return(true);
            }
        }

        return(false);                
    }
    else if(EyeDirection == E)
    {            
        if((AbsDiff(EntityPos.X,EyePos.X) > EyeDepth) ||
           (EntityPos.X < EyePos.X) ||
           (AbsDiff(EntityPos.Y,EyePos.Y) > ((EyeWidth/2))))
        {
            return(false);
        }

        real32 NoseToEye = 0.75f;
        uint32 CornerAmount = 0;
        real32 LeftSlope = (EyeWidth/2)/EyeDepth;
        real32 RightSlope = -(EyeWidth/2)/EyeDepth;
        real32 LeftB = (EyePos.Y + NoseToEye) - (LeftSlope*EyePos.X);
        real32 RightB = (EyePos.Y - NoseToEye) - (RightSlope*EyePos.X);
        
        if((EntityPos.X > EyePos.X) &&
           ((EntityPos.X - EyePos.X) < EyeDepth))
        {
            real32 LeftYAtCornerX = EntityPos.X*LeftSlope + LeftB;
            real32 RightYAtCornerX = EntityPos.X *RightSlope + RightB;

            if((EntityPos.Y < LeftYAtCornerX) && (EntityPos.Y > RightYAtCornerX))
            {
                ++CornerAmount;        
            }
        }                
        

        if(!CornerAmount)
        {
            return(false);
        }
        
        if(AbsDiff(EntityPos.X, EyePos.X) < 0.99f &&
           (Tile[((uint32)EyePos.Y * 45) + (uint32)EyePos.X] != WALL_TILE) &&
           (Tile[((uint32)EntityPos.Y * 45) + (uint32)EntityPos.X] != WALL_TILE))
        {
            return(true);
        }
                
        v2 Eye[3] =
            {
                {EyePos.X, EyePos.Y + NoseToEye},
                {EyePos.X, EyePos.Y},
                {EyePos.X, EyePos.Y - NoseToEye}
            };
        
        bool32 EyeObstructed[3] = {};
        uint32 TileID = 0;
        real32 YAtX = 0;
        real32 EyeSlope = 0;
        real32 EyeB = 0;
        
        for(uint32 EyeI = 0; EyeI < 3; ++EyeI)
        {
            EyeSlope = (EntityPos.Y - Eye[EyeI].Y)/(EntityPos.X - Eye[EyeI].X);
            EyeB = Eye[EyeI].Y - EyeSlope*Eye[EyeI].X;
            
            for(uint32 UnitStep = (uint32)Eye[EyeI].X; UnitStep < EntityPos.X; ++UnitStep)
            {

                //TODO: This was causing XAtY to be some large negative number because of the slope being so high.
                //Possibly fix by saying that if slope is > 100(or some appropriate number, just make XAtY be equal to EntityPos.X
                
                YAtX = EyeSlope*UnitStep + EyeB;

                if(YAtX < 0 && YAtX > 50)
                {
                    //TODO
                    continue;
                }
                   
                TileID = Tile[((uint32)YAtX * 45) + UnitStep];
                
                if(TileID == WALL_TILE || TileID == OUTER_WALL_TILE)
                {
                    EyeObstructed[EyeI] = true;
                    break;
                }
            }
            
            if(!EyeObstructed[EyeI])
            {
                return(true);
            }
        }

        return(false);                
    }
    else if(EyeDirection == W)
    {
        if((AbsDiff(EntityPos.X,EyePos.X) > EyeDepth) ||
           (EntityPos.X > EyePos.X) ||
           (AbsDiff(EntityPos.Y,EyePos.Y) > ((EyeWidth/2))))
        {
            return(false);
        }

        real32 NoseToEye = 0.75f;
        bool32 CornerAmount = 0;
        real32 LeftSlope = (EyeWidth/2)/EyeDepth;
        real32 RightSlope = -(EyeWidth/2)/EyeDepth;
        real32 LeftB = (EyePos.Y - NoseToEye) - (LeftSlope*EyePos.X);
        real32 RightB = (EyePos.Y + NoseToEye) - (RightSlope*EyePos.X);
        
        if((EntityPos.X < EyePos.X) &&
           ((EyePos.X - EntityPos.X) < EyeDepth))
        {
            real32 LeftYAtCornerX = EntityPos.X*LeftSlope + LeftB;
            real32 RightYAtCornerX = EntityPos.X *RightSlope + RightB;

            if((EntityPos.Y > LeftYAtCornerX) && (EntityPos.Y < RightYAtCornerX))
            {
                ++CornerAmount;
            }
        }                
        
        if(!CornerAmount)
        {
            return(false);
        }
        
        if(AbsDiff(EntityPos.X, EyePos.X) < 0.99f &&
           (Tile[((uint32)EyePos.Y * 45) + (uint32)EyePos.X] != WALL_TILE) &&
           (Tile[((uint32)EntityPos.Y * 45) + (uint32)EntityPos.X] != WALL_TILE))
        {
            return(true);
        }
                
        v2 Eye[3] =
            {
                {EyePos.X, EyePos.Y - NoseToEye},
                {EyePos.X, EyePos.Y},
                {EyePos.X, EyePos.Y + NoseToEye}
            };
        
        bool32 EyeObstructed[3] = {};
        uint32 TileID = 0;
        real32 YAtX = 0;
        real32 EyeSlope = 0;
        real32 EyeB = 0;
        
        for(uint32 EyeI = 0; EyeI < 3; ++EyeI)
        {
            EyeSlope = (EntityPos.Y - Eye[EyeI].Y)/(EntityPos.X - Eye[EyeI].X);
            EyeB = Eye[EyeI].Y - EyeSlope*Eye[EyeI].X;

                for(uint32 UnitStep = (uint32)Eye[EyeI].X; UnitStep > (uint32)(EntityPos.X - 1); --UnitStep)
            {

                //TODO: This was causing XAtY to be some large negative number because of the slope being so high.
                //Possibly fix by saying that if slope is > 100(or some appropriate number, just make XAtY be equal to EntityPos.X
                
                YAtX = EyeSlope*UnitStep + EyeB;

                if(YAtX < 0 && YAtX > 50)
                {
                    //TODO
                    continue;
                }
                
                TileID = Tile[((uint32)YAtX * 45) + UnitStep];
                
                if(TileID == WALL_TILE || TileID == OUTER_WALL_TILE)
                {
                    EyeObstructed[EyeI] = true;
                    break;
                }
            }
            
            if(!EyeObstructed[EyeI])
            {
                return(true);
            }
        }

        return(false);                
    }        
    return(false);            
}

    

uint8 *
PathToExit(room *RoomA, hub *HubA, uint8 *TempPath, uint32 TempPathI, uint64 ExitMask)
{
    hub HubZ = {};
    
    for(uint32 i2 = 0; i2 < RoomA->HubCount; ++i2)
    {
        if((RoomA->Hub[i2].ID & ExitMask) == 0)
        {
            HubZ = RoomA->Hub[i2];
            break;
        }
    }

    uint64 OtherDoorMask[8] = {};
            
    for(uint32 RoomI = 0; RoomI < RoomA->AdjacentRoomCount; ++RoomI)
    {
        if(ExitMask == ((0x0F << 8*RoomI) & RoomA->DoorMask))
        {
                    
        }
        else
        {
            OtherDoorMask[RoomI] = ((0x0F << 8*RoomI) & RoomA->DoorMask);    
        }
    }
    
    uint32 LoopCount = 0;
    Assert(HubZ.Char != 0);
    uint64 OneMask = 0x1111111111111111;
    while(HubA->Char != HubZ.Char)
    {
        if(++LoopCount > 50)
        {
            Assert(0);
        }
        uint64 HubsAway = (HubA->ID & ExitMask);

        for(uint32 i3 = 0; i3 < RoomA->HubCount; ++i3)
        {
            uint64 iHubsAway = (RoomA->Hub[i3].ID & ExitMask);
            if(iHubsAway != (HubsAway - (OneMask & ExitMask)))
            {
                continue;
            }
            

            bool32 HubIsAdjacent = true;
            for(uint32 ODMI = 0; ODMI < (RoomA->AdjacentRoomCount - 1); ++ODMI)
            {
                
                if((((int32)(OtherDoorMask[ODMI] & RoomA->Hub[i3].ID) -
                     (int32)(OtherDoorMask[ODMI] & HubA->ID)) > (int32)(OneMask & OtherDoorMask[ODMI])) ||
                   (((int32)(OtherDoorMask[ODMI] & HubA->ID) -
                    (int32)(OtherDoorMask[ODMI] & RoomA->Hub[i3].ID)) > (int32)(OneMask & OtherDoorMask[ODMI])
                    ))
                {
                    HubIsAdjacent = false;
                    break;
                }
            }
            
            if(((RoomA->Hub[i3].Pos.X + 0.05f > HubA->Pos.X &&
                 RoomA->Hub[i3].Pos.X - 0.05f < HubA->Pos.X) ||                 
                (RoomA->Hub[i3].Pos.Y + 0.05f > HubA->Pos.Y &&
                 RoomA->Hub[i3].Pos.Y - 0.05f < HubA->Pos.Y)) &&
               HubIsAdjacent)
            {
                TempPath[TempPathI++] = RoomA->Hub[i3].Char;
                HubA = &RoomA->Hub[i3];
                break;
            }               
        }        
    }
    return(TempPath);
}

hub
GetHub(room *Room, char HubChar)
{
    hub Result = {};
    for(uint32 HubI = 0; HubI < Room->HubCount; ++HubI)
    {
        if(Room->Hub[HubI].Char == HubChar)
        {
            Result = Room->Hub[HubI];
            break;
        }
    }
    return(Result);           
}


//013
inline v2
GetNPCSpeedVector(v2 A, v2 B)
{
    v2 Result = {};
    
    real32 SlopeY = B.Y - A.Y;
    real32 SlopeX = B.X - A.X;
        
    if(SlopeY*SlopeY > SlopeX*SlopeX)
    {
        if(SlopeY < 0)
        {
            Result.Y = -1;
            Result.X = SlopeX/SlopeY*-1.0f;
        }
        else if(SlopeY > 0)
        {
            Result.Y = 1;
            Result.X = SlopeX/SlopeY;
        }
            
    }
    else if(SlopeX*SlopeX > SlopeY*SlopeY)
    {
        if(SlopeX < 0)
        {
            Result.X = -1;
            Result.Y = SlopeY/SlopeX*-1.0f;
        }
        else if(SlopeX > 0)
        {
            Result.X = 1;
            Result.Y = SlopeY/SlopeX;
        }
    }


    return(Result);
}

uint32
ClearPathCheckAndFalseHubAssign(v2 StartPos, v2 EndPos, v2 *FalseHubPos, uint32 *AptTileMap)
{
    bool32 CanHubSkip = true;            
    bool32 HaveSameX = false;
    bool32 HaveSameY = false;
   

    uint32 ResultBitFlag = 0;

    real32 NorthLimit = 0.0f;
    real32 SouthLimit = 0.0f;
    real32 EastLimit = 0.0f;
    real32 WestLimit = 0.0f;
                                
    if(StartPos.X > EndPos.X)
    {
        //Check StartPos.Y and EndPos.Y along EndPos.X
        //HeadWest = true;
        WestLimit = EndPos.X;
        EastLimit = StartPos.X - 1;
    }
    else
    {
        //Check StartPos.Y and EndPos.Y along EndPos.X
        WestLimit = StartPos.X + 1;
        EastLimit = EndPos.X;
    }

    if(StartPos.Y > EndPos.Y)
    {
        //Check StartPos.X and EndPos.X along EndPos.Y
        //HeadSouth = true;
        SouthLimit = EndPos.Y;
        NorthLimit = StartPos.Y - 1;
    }
    else
    {
        //Check StartPos.X and EndPos.X along EndPos.Y
        SouthLimit = StartPos.Y + 1;
        NorthLimit = EndPos.Y;
    }
            
    if(AbsDiff(StartPos.X, EndPos.X) < 0.1f)               
    {
        //They are adjacent, check for wall.
        HaveSameX = true;
        for(real32 UnitStep = SouthLimit; (uint32)UnitStep <= (uint32)NorthLimit; ++UnitStep)
        {
                        
            if(IsObstacle(AptTileMap[((uint32)UnitStep * 45) + (uint32)EndPos.X]))//[(uint32)UnitStep][(uint32)EndPos.X]))
            {
                if((uint32)UnitStep != (uint32)EndPos.Y)
                {
                    CanHubSkip = false;
                    break; 
                }
                
            }
        }
                
    }
            
    if(CanHubSkip && (AbsDiff(StartPos.Y, EndPos.Y) < 0.1f))
    {
        HaveSameY = true;
        //They are adjacent, check for wall.
        for(real32 UnitStep = WestLimit; (uint32)UnitStep <= (uint32)EastLimit; ++UnitStep)
        {
                        
            if(IsObstacle(AptTileMap[((uint32)EndPos.Y * 45) + (uint32)UnitStep]))
            {
                if((uint32)UnitStep != (uint32)EndPos.X)
                {
                    CanHubSkip = false;
                    break; 
                }
            }
        }
    }
                        
    if(CanHubSkip && !HaveSameX && !HaveSameY)
    {

        bool32 AXtoBXbyAY = true;
        bool32 AXtoBXbyBY = true;
        bool32 AYtoBYbyAX = true;
        bool32 AYtoBYbyBX = true;
        //bool32 HeadSouth = false;
        //bool32 HeadWest = false;
                
                                                                                                                    
        for(real32 UnitStep = WestLimit; (uint32)UnitStep <= (uint32)EastLimit; ++UnitStep)
        {
                        
            if(IsObstacle(AptTileMap[((uint32)StartPos.Y * 45) + (uint32)UnitStep]) &&
               ((uint32)UnitStep != (uint32)EndPos.X))/*GameState->AptTileMap[(uint32)StartPos.Y][(uint32)UnitStep]*/
            {
                
                AXtoBXbyAY = false;
                break;
            }
        }

        if(AXtoBXbyAY)
        {
            for(real32 UnitStep = SouthLimit; (uint32)UnitStep <= (uint32)NorthLimit; ++UnitStep)
            {
                        
                if(IsObstacle(AptTileMap[((uint32)UnitStep * 45) + (uint32)EndPos.X]) &&
                   ((uint32)UnitStep != (uint32)EndPos.Y))
                {
                    AYtoBYbyBX = false;
                    break;
                }
            }
        }

        if(!AXtoBXbyAY || !AYtoBYbyBX)
        {
            for(real32 UnitStep = SouthLimit; (uint32)UnitStep <= (uint32)NorthLimit; ++UnitStep)
            {
                        
                if(IsObstacle(AptTileMap[((uint32)UnitStep * 45) + (uint32)StartPos.X]) &&
                   ((uint32)UnitStep != (uint32)EndPos.Y))
                {
                    AYtoBYbyAX = false;
                    break;
                }
            }

            if(AYtoBYbyAX)
            {
                for(real32 UnitStep = WestLimit; (uint32)UnitStep <= (uint32)EastLimit; ++UnitStep)
                {
                        
                    if(IsObstacle(AptTileMap[((uint32)EndPos.Y * 45) + (uint32)UnitStep]) &&
                       ((uint32)UnitStep != (uint32)EndPos.X))
                    {
                        
                        AXtoBXbyBY = false;
                        break;
                    }
                    
                }
            }
        }
                
        if(AXtoBXbyAY && AYtoBYbyBX)
        {
            //we can do the hub skip
            FalseHubPos->X = EndPos.X;
            FalseHubPos->Y = StartPos.Y;
                    
        }
        else if(AYtoBYbyAX && AXtoBXbyBY)
        {
            //we can do the hub skip
            FalseHubPos->X = StartPos.X;
            FalseHubPos->Y = EndPos.Y;                    
        }
        else
        {
            CanHubSkip = false;
        }
    }

    if(CanHubSkip)
    {
        ResultBitFlag += (1 << CAN_HUB_SKIP);
    }

    if(HaveSameX)
    {
        ResultBitFlag += (1 << HAVE_SAME_X);
    }

    if(HaveSameY)
    {
        ResultBitFlag += (1 << HAVE_SAME_Y);
    }
    
    
    return(ResultBitFlag);
}

bool32
SelectItemCheck(v2 StartPos, v2 EndPos, uint32 *AptTileMap)
{
    bool32 CanHubSkip = true;            
    bool32 HaveSameX = false;
    bool32 HaveSameY = false;

    real32 NorthLimit = 0.0f;
    real32 SouthLimit = 0.0f;
    real32 EastLimit = 0.0f;
    real32 WestLimit = 0.0f;
                                
    if(StartPos.X > EndPos.X)
    {
        //Check StartPos.Y and EndPos.Y along EndPos.X
        //HeadWest = true;
        WestLimit = EndPos.X;
        EastLimit = StartPos.X - 1;
    }
    else
    {
        //Check StartPos.Y and EndPos.Y along EndPos.X
        WestLimit = StartPos.X + 1;
        EastLimit = EndPos.X;
    }

    if(StartPos.Y > EndPos.Y)
    {
        //Check StartPos.X and EndPos.X along EndPos.Y
        //HeadSouth = true;
        SouthLimit = EndPos.Y;
        NorthLimit = StartPos.Y - 1;
    }
    else
    {
        //Check StartPos.X and EndPos.X along EndPos.Y
        SouthLimit = StartPos.Y + 1;
        NorthLimit = EndPos.Y;
    }
            
    if((uint32)StartPos.X == (uint32)EndPos.X)               
    {
        //They are adjacent, check for wall.
        HaveSameX = true;
        for(real32 UnitStep = SouthLimit; (uint32)UnitStep <= (uint32)NorthLimit; ++UnitStep)
        {
                        
            if(AptTileMap[((uint32)UnitStep * 45) + (uint32)EndPos.X] == WALL_TILE)//[(uint32)UnitStep][(uint32)EndPos.X]))
            {
                if((uint32)UnitStep != (uint32)EndPos.Y)
                {
                    CanHubSkip = false;
                    break; 
                }
                
            }
        }
                
    }
            
    if(CanHubSkip)
    {
        HaveSameY = true;
        //They are adjacent, check for wall.
        for(real32 UnitStep = WestLimit; (uint32)UnitStep <= (uint32)EastLimit; ++UnitStep)
        {
                        
            if(AptTileMap[((uint32)EndPos.Y * 45) + (uint32)UnitStep] == WALL_TILE)
            {
                if((uint32)UnitStep != (uint32)EndPos.X)
                {
                    CanHubSkip = false;
                    break; 
                }
            }
        }
    }
                        
    if(CanHubSkip && !HaveSameX && !HaveSameY)
    {

        bool32 AXtoBXbyAY = true;
        bool32 AXtoBXbyBY = true;
        bool32 AYtoBYbyAX = true;
        bool32 AYtoBYbyBX = true;
        //bool32 HeadSouth = false;
        //bool32 HeadWest = false;
                
                                                                                                                    
        for(real32 UnitStep = WestLimit; (uint32)UnitStep <= (uint32)EastLimit; ++UnitStep)
        {
                        
            if((WALL_TILE == AptTileMap[((uint32)StartPos.Y * 45) + (uint32)UnitStep]) &&
               ((uint32)UnitStep != (uint32)EndPos.X))/*GameState->AptTileMap[(uint32)StartPos.Y][(uint32)UnitStep]*/
            {
                
                AXtoBXbyAY = false;
                break;
            }
        }

        if(AXtoBXbyAY)
        {
            for(real32 UnitStep = SouthLimit; (uint32)UnitStep <= (uint32)NorthLimit; ++UnitStep)
            {
                        
                if((WALL_TILE == AptTileMap[((uint32)UnitStep * 45) + (uint32)EndPos.X]) &&
                   ((uint32)UnitStep != (uint32)EndPos.Y))
                {
                    AYtoBYbyBX = false;
                    break;
                }
            }
        }

        if(!AXtoBXbyAY || !AYtoBYbyBX)
        {
            for(real32 UnitStep = SouthLimit; (uint32)UnitStep <= (uint32)NorthLimit; ++UnitStep)
            {
                        
                if((WALL_TILE == AptTileMap[((uint32)UnitStep * 45) + (uint32)StartPos.X]) &&
                   ((uint32)UnitStep != (uint32)EndPos.Y))
                {
                    AYtoBYbyAX = false;
                    break;
                }
            }

            if(AYtoBYbyAX)
            {
                for(real32 UnitStep = WestLimit; (uint32)UnitStep <= (uint32)EastLimit; ++UnitStep)
                {
                        
                    if((WALL_TILE == AptTileMap[((uint32)EndPos.Y * 45) + (uint32)UnitStep]) &&
                       ((uint32)UnitStep != (uint32)EndPos.X))
                    {
                        
                        AXtoBXbyBY = false;
                        break;
                    }
                    
                }
            }
        }
        if(!((AXtoBXbyAY && AYtoBYbyBX) || (AYtoBYbyAX && AXtoBXbyBY)))
        {        
            CanHubSkip = false;
        }
    }
    
    
    
    return(CanHubSkip);
    
}

v2
AdjustDest(v2 ProposedDest, uint32 *LevelMap)
{
    v2 ValidDest = {};
    uint32 TileID = 0;

    v2 PotentialPos[12] =
        {
            {(uint32)ProposedDest.X+1.01f, ProposedDest.Y},
            {(uint32)ProposedDest.X+2.01f, ProposedDest.Y},
            {(uint32)ProposedDest.X-.01f, ProposedDest.Y},
            {(uint32)ProposedDest.X-1.01f, ProposedDest.Y},
            {(uint32)(ProposedDest.X+1) + 0.01f, (uint32)(ProposedDest.Y+1) + 0.01f},
            {(uint32)(ProposedDest.X+1) + 0.01f, (uint32)(ProposedDest.Y) - 0.01f},
            {(uint32)(ProposedDest.X) - 0.01f, (uint32)(ProposedDest.Y) - 0.01f},
            {(uint32)(ProposedDest.X) - 0.01f, (uint32)(ProposedDest.Y + 1) + 0.01f},
            {ProposedDest.X, (uint32)ProposedDest.Y+1.01f},
            {ProposedDest.X, (uint32)ProposedDest.Y+2.01f},
            {ProposedDest.X, (uint32)ProposedDest.Y-.01f},
            {ProposedDest.X, (uint32)ProposedDest.Y-1.01f}            
        };

    v2 ValidPos[12] = {};
    uint32 VI = 0;
    
    for(uint32 DI = 0; DI < 12; ++DI)
    {
        if(PotentialPos[DI].Y >= 0 &&
           PotentialPos[DI].Y < 50 &&
           PotentialPos[DI].X >=0 &&
           PotentialPos[DI].X < 46 &&
           !IsObstacle(LevelMap[((uint32)PotentialPos[DI].Y * 45) + (uint32)(PotentialPos[DI].X)]))
        {
            ValidPos[VI] = PotentialPos[DI];
            ++VI;
        }
    }

    if(VI)
    {        
        real32 NearestLength = 1000.0f;
        real32 ThisLength = 0.0f;
        
        for(uint32 PI = 0; PI < VI; ++PI)
        {
            ThisLength = LengthSq(v2{ProposedDest.X - ValidPos[PI].X, ProposedDest.Y - ValidPos[PI].Y});
            if(ThisLength < NearestLength)
            {
                NearestLength = ThisLength;
                ValidDest = ValidPos[PI];
            }
        }   
    }
    else
    {
        Assert(0);
    }
    
    return(ValidDest);        
}
    
tile
TipToe(v2 StartPos, v2 EndPos, uint32 Direction, bool32 CheckForNonCollision, uint32 *LevelMap)
{        
    tile SearchTile = {};
    SearchTile.Value = MAX_TILE_TYPE;
    SearchTile.Pos = StartPos;
    uint32 TileValue = 0;
    bool32 FoundTile = false;
    
    switch(Direction)
    {
        
        case N:
        {
            for(real32 UnitStep = StartPos.Y + 1; (uint32)UnitStep <= (uint32)EndPos.Y; ++UnitStep)
            {
                TileValue = LevelMap[((uint32)UnitStep * 45) + (uint32)StartPos.X];
                if(TileValue != FLOOR_TILE && (CheckForNonCollision || (TileValue == DOOR_TILE || IsObstacle(TileValue))))
                {                    
                    SearchTile.Pos.Y = UnitStep;
                    FoundTile = true;
                    break;
                }                            
            }
        }break;

        case S:
        {
            for(real32 UnitStep = StartPos.Y - 1; (uint32)UnitStep >= (uint32)EndPos.Y; --UnitStep)
            {
                TileValue = LevelMap[((uint32)UnitStep * 45) + (uint32)StartPos.X];
                if(TileValue != FLOOR_TILE && (CheckForNonCollision || (TileValue == DOOR_TILE || IsObstacle(TileValue))))
                {                    
                    SearchTile.Pos.Y = UnitStep;
                    FoundTile = true;
                    break;
                }                                            
            }
        }break;

        case E:
        {
            for(real32 UnitStep = StartPos.X + 1; (uint32)UnitStep <= (uint32)EndPos.X; ++UnitStep)
            {
                TileValue = LevelMap[((uint32)StartPos.Y * 45) + (uint32)UnitStep];
                if(TileValue != FLOOR_TILE && (CheckForNonCollision || (TileValue == DOOR_TILE || IsObstacle(TileValue))))
                {                    
                    SearchTile.Pos.X = UnitStep;
                    FoundTile = true;
                    break;
                }                                            
            }
        }break;

        case W:
        {
            for(real32 UnitStep = StartPos.X - 1; (uint32)UnitStep >= (uint32)EndPos.X; --UnitStep)
            {
                TileValue = LevelMap[((uint32)StartPos.Y * 45) + (uint32)UnitStep];
                if(TileValue != FLOOR_TILE && (CheckForNonCollision || (TileValue == DOOR_TILE || IsObstacle(TileValue))))
                {                    
                    SearchTile.Pos.X = UnitStep;
                    FoundTile = true;
                    break;
                }                
            }
        }break;
        default:
        {
            Assert(0);
        }
    };
    
    if(FoundTile)
    {           
        SearchTile.Value = TileValue;
    }
    else
    {
        SearchTile.Pos = EndPos;
    }

    return(SearchTile);    
}

    
#define GHOST_PATH_H
#endif

/*
uint8 *
PathToExit(board *BoardA, hub *HubA, uint8 *TempPath, uint32 TempPathI, uint32 ExitMask) 
{

    
    hub HubZ = {};
    for(uint32 i2 = 0; i2 < BoardA->HubCount; ++i2)
    {
        if((BoardA->Hub[i2].ID & ExitMask) == 0)
        {
            HubZ = BoardA->Hub[i2];
            break;
        }
    }

    uint32 LoopCount = 0;
    Assert(HubZ.Char != 0);
    while(HubA->Char != HubZ.Char)                        
    {

        uint32 HubsAway = (HubA->ID & (ExitMask));
        for(uint32 i3 = 0; i3 < BoardA->HubCount; ++i3)
        {
            uint32 iHubsAway = (BoardA->Hub[i3].ID & (ExitMask));
            if(iHubsAway != (HubsAway - (0x11111 & ExitMask)))
            {
                continue;
            }


            // 1111 0000
            // 0001 0000
            //
            //Find which of the closer hubs is adjacent to current hub.
            //IMPORTANT: Current checking strategy is not failproof
            //TODO: Literally compare hub positions to see which one is adjacent            
            uint32 OtherDoorMask = (BoardA->DoorMask ^ ExitMask);
            if((((OtherDoorMask & BoardA->Hub[i3].ID) - (OtherDoorMask & HubA->ID)) ==
                (OtherDoorMask - (OtherDoorMask & 0xEEEEE)) ||
                (((OtherDoorMask & HubA->ID) - (OtherDoorMask & BoardA->Hub[i3].ID)) ==
                 (OtherDoorMask - (OtherDoorMask & 0xEEEEE)))) &&
               ((BoardA->Hub[i3].Pos.X + 0.05f > HubA->Pos.X &&
                 BoardA->Hub[i3].Pos.X - 0.05f < HubA->Pos.X) ||                 
                (BoardA->Hub[i3].Pos.Y + 0.05f > HubA->Pos.Y &&
                BoardA->Hub[i3].Pos.Y - 0.05f < HubA->Pos.Y)))
            {
                TempPath[TempPathI++] = BoardA->Hub[i3].Char;
                HubA = &BoardA->Hub[i3];
                break;
            }
        }
        if(LoopCount == 50)
        {
            LoopCount = 51;
        }
    }
    return(TempPath);
}
*/
