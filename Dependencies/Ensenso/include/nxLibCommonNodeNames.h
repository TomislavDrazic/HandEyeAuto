#ifndef __COMMON_NODENAMES_H__
#define __COMMON_NODENAMES_H__

#ifdef __cli
#	define COMMON_ITEM(NAME) static String ^ itm##NAME = #    NAME
#	define COMMON_VALUE(NAME) static String ^ val##NAME = #    NAME
#else
#	ifndef OVERRIDE_NXLIB_CONSTANTS
#		define COMMON_ITEM(NAME) static char const* const itm##NAME = #        NAME
#		define COMMON_VALUE(NAME) static char const* const val##NAME = #        NAME
#	endif
#endif /* __cli */

COMMON_ITEM(Value);
COMMON_ITEM(Protection);
COMMON_ITEM(ExtendedType);
COMMON_ITEM(BinaryInfo);
COMMON_ITEM(Info);
COMMON_ITEM(MetaInfo);
COMMON_ITEM(Name);
COMMON_ITEM(MetaData);
COMMON_ITEM(Factory);
COMMON_ITEM(User);
COMMON_ITEM(Url);
COMMON_ITEM(Default);
COMMON_ITEM(Values);
// COMMON_ITEM(Type);
COMMON_ITEM(Min);
COMMON_ITEM(Max);
COMMON_ITEM(Interval);
COMMON_ITEM(Options);

COMMON_ITEM(Time);
COMMON_ITEM(Epipolar);

COMMON_ITEM(Filename);
COMMON_ITEM(Type);
COMMON_VALUE(Sphere);
COMMON_VALUE(Cylinder);
COMMON_VALUE(Cube);
COMMON_VALUE(Cuboid);
COMMON_VALUE(Box);
COMMON_VALUE(TPiece);
COMMON_ITEM(Radius);
COMMON_ITEM(Width);
COMMON_ITEM(Depth);
COMMON_ITEM(Height);
COMMON_ITEM(Fixed);
COMMON_ITEM(Mass);
COMMON_ITEM(InitialVelocity);
COMMON_ITEM(Lighting);
COMMON_ITEM(Color);
COMMON_ITEM(Opacity);
COMMON_ITEM(Ambient);
COMMON_ITEM(Diffuse);
COMMON_ITEM(Specular);
COMMON_ITEM(Shininess);
COMMON_ITEM(MaterialBlur);
COMMON_ITEM(PointMap);
COMMON_ITEM(Lines);

COMMON_ITEM(PartFinderViewpoints);
COMMON_ITEM(BoundingBox);
COMMON_ITEM(Cluster);
COMMON_ITEM(Duplicate);
COMMON_ITEM(MinimumAspectRatio);
COMMON_ITEM(Viewpoints);
COMMON_ITEM(Distance);
COMMON_ITEM(AngularResolution);
COMMON_ITEM(InPlaneAngularResolution);
COMMON_ITEM(RelativeModelSamplingDistance);
COMMON_ITEM(CoverageDistanceThreshold);
COMMON_ITEM(Symmetries);
COMMON_ITEM(Order);
COMMON_ITEM(Texture);
COMMON_ITEM(Subsampling);

COMMON_ITEM(Transformation);
COMMON_ITEM(Inverse);
COMMON_ITEM(Rotation);
COMMON_ITEM(Axes);
COMMON_ITEM(Axis);
COMMON_ITEM(Convention);
COMMON_ITEM(Extrinsic);
COMMON_ITEM(Angles);
COMMON_ITEM(Angle);
COMMON_ITEM(Translation);
COMMON_ITEM(Scaling);
COMMON_ITEM(Pose);
COMMON_ITEM(Link);
COMMON_ITEM(ResolvedLink);
COMMON_ITEM(Reference);
COMMON_ITEM(Source);
COMMON_ITEM(Target);

COMMON_VALUE(XYZ);
COMMON_VALUE(ZYX);
COMMON_VALUE(ZYZ);

COMMON_VALUE(ABB);
COMMON_VALUE(Adept);
COMMON_VALUE(Comau);
COMMON_VALUE(Doosan);
COMMON_VALUE(Epson);
COMMON_VALUE(Fanuc);
COMMON_VALUE(Fruitcore);
COMMON_VALUE(Kawasaki);
COMMON_VALUE(Kuka);
COMMON_VALUE(Mecademic);
COMMON_VALUE(Mitsubishi);
COMMON_VALUE(Nachi);
COMMON_VALUE(Staeubli);
COMMON_VALUE(Techman);
COMMON_VALUE(Yaskawa);

COMMON_ITEM(RayTracing);
COMMON_ITEM(Enabled);
COMMON_ITEM(FireflyThreshold);
COMMON_ITEM(Samples);
COMMON_ITEM(Count);
COMMON_ITEM(MaximumDepth);
COMMON_ITEM(RussianRoulette);
COMMON_ITEM(Interrupt);
COMMON_ITEM(MinimumProbability);
COMMON_ITEM(MinimumDepth);

COMMON_ITEM(Polarity);
COMMON_ITEM(GridSpacing);
COMMON_ITEM(GridSize);
COMMON_ITEM(Thickness);
COMMON_ITEM(Offset);
COMMON_ITEM(AssemblyPattern);
COMMON_ITEM(AssemblyCalibration);
COMMON_ITEM(Binning);
COMMON_ITEM(BinningShift);
COMMON_ITEM(ImageOffset);

COMMON_ITEM(Invert);
COMMON_ITEM(And);
COMMON_ITEM(UseModel);
COMMON_ITEM(Cameras);
COMMON_ITEM(Or);
COMMON_ITEM(Reduce);

COMMON_ITEM(K1);
COMMON_ITEM(K2);
COMMON_ITEM(K3);
COMMON_ITEM(T1);
COMMON_ITEM(T2);
COMMON_ITEM(Rx);
COMMON_ITEM(Ry);

COMMON_ITEM(Execute);
// COMMON_ITEM(Default);
COMMON_ITEM(Command);
COMMON_ITEM(Parameters);
COMMON_ITEM(Result);

COMMON_ITEM(Board);
COMMON_ITEM(LED);
COMMON_ITEM(CPU);
COMMON_ITEM(Camera);
COMMON_ITEM(Projector);

COMMON_ITEM(Wifi);
COMMON_ITEM(Mode);
COMMON_VALUE(AccessPoint);
COMMON_VALUE(Client);
COMMON_ITEM(Status);
COMMON_VALUE(Enabled);
COMMON_VALUE(Disabled);
COMMON_VALUE(Connected);
COMMON_VALUE(Disconnected);
COMMON_VALUE(Scanning);
COMMON_ITEM(AccessPoint);
COMMON_ITEM(Client);
COMMON_ITEM(Ssid);
COMMON_ITEM(Channel);
COMMON_ITEM(Frequency);
COMMON_VALUE(Wpa2Psk);
COMMON_ITEM(Passphrase);

COMMON_VALUE(None);
COMMON_VALUE(All);
COMMON_VALUE(Left);
COMMON_VALUE(Right);
COMMON_VALUE(Down);
COMMON_VALUE(Up);
COMMON_VALUE(Locked);
COMMON_VALUE(StructureLocked);
COMMON_VALUE(NotSpecified);

COMMON_VALUE(Standard);
COMMON_VALUE(Link);
COMMON_VALUE(Binary);
COMMON_VALUE(LinkHidden);
COMMON_VALUE(Hidden);

COMMON_VALUE(Software);
COMMON_VALUE(Continuous);
COMMON_VALUE(RisingEdge);
COMMON_VALUE(FallingEdge);
COMMON_VALUE(Internal);

COMMON_VALUE(Input);
COMMON_VALUE(Low);
COMMON_VALUE(High);
COMMON_VALUE(HighActive);
COMMON_VALUE(LowActive);
COMMON_VALUE(Off);
COMMON_VALUE(PWM);
COMMON_VALUE(MirrorFlashOutput);

COMMON_VALUE(DiscardNew);
COMMON_VALUE(DiscardOld);

COMMON_VALUE(USB);
COMMON_VALUE(Ethernet);
COMMON_VALUE(File);
COMMON_VALUE(Virtual);
COMMON_VALUE(Local);
COMMON_VALUE(Remote);
COMMON_VALUE(Unknown);

COMMON_VALUE(Null);
COMMON_VALUE(Number);
COMMON_VALUE(String);
COMMON_VALUE(Boolean);
COMMON_VALUE(Array);
COMMON_VALUE(Object);

COMMON_VALUE(Error);
COMMON_VALUE(Warning);
COMMON_VALUE(Info);
COMMON_VALUE(Debug);
COMMON_VALUE(Trace);

COMMON_VALUE(Ignore);
COMMON_VALUE(Force);
COMMON_VALUE(Allow);

COMMON_VALUE(Single);
COMMON_VALUE(SingleCustom);
COMMON_VALUE(Flexible);
COMMON_VALUE(Assembly);
COMMON_VALUE(GridSpacing);
COMMON_VALUE(Static);
COMMON_VALUE(TiltDirection);
COMMON_VALUE(Mask);
COMMON_VALUE(Index);
COMMON_VALUE(GridWidth);
COMMON_VALUE(GridHeight);
COMMON_VALUE(Type);
COMMON_VALUE(Monocular);
COMMON_VALUE(SerialNumber);

COMMON_VALUE(Random);
COMMON_VALUE(FlexView2);
COMMON_VALUE(StructuredLight);
COMMON_VALUE(FlexView3);

COMMON_VALUE(SgmAligned);
COMMON_VALUE(SgmDiagonal);
COMMON_VALUE(SgmAlignedAndDiagonal);
COMMON_VALUE(SgmSinglePass);
COMMON_VALUE(Correlation);
COMMON_VALUE(BlockMatching);
COMMON_VALUE(PatchMatch);

COMMON_VALUE(Raw);
COMMON_VALUE(Rectified);

COMMON_VALUE(Auto);
COMMON_VALUE(DHCP);
COMMON_VALUE(Persistent);

COMMON_ITEM(RenderingMode);
COMMON_ITEM(PointSize);
COMMON_ITEM(BillboardSize);
COMMON_ITEM(ColoringMode);
COMMON_ITEM(ColorRepetitionDistance);
COMMON_ITEM(ColorOffset);
COMMON_ITEM(MixColorWithTexture);
COMMON_ITEM(ColorProportion);
COMMON_ITEM(OverlayColorWithMonoTexture);

COMMON_VALUE(Mesh);
COMMON_VALUE(Points);
COMMON_VALUE(Squares);
COMMON_VALUE(Spheres);
COMMON_VALUE(Boxes);

COMMON_VALUE(Texture);
COMMON_VALUE(WorldZ);
COMMON_VALUE(CameraZ);
COMMON_VALUE(Uniform);

COMMON_VALUE(Block);
COMMON_VALUE(Streaming);

COMMON_ITEM(Pattern);
COMMON_ITEM(Brightness);
COMMON_ITEM(Contrast);
COMMON_ITEM(Points);

COMMON_ITEM(ImageSize);

COMMON_VALUE(X);
COMMON_VALUE(XPositive);
COMMON_VALUE(XNegative);
COMMON_VALUE(XRotationSymmetric);
COMMON_VALUE(Y);
COMMON_VALUE(YPositive);
COMMON_VALUE(YNegative);
COMMON_VALUE(YRotationSymmetric);
COMMON_VALUE(Z);
COMMON_VALUE(ZPositive);
COMMON_VALUE(ZNegative);
COMMON_VALUE(ZRotationSymmetric);

COMMON_VALUE(Normal);
// COMMON_VALUE(Hidden);
COMMON_VALUE(UserData);

#undef COMMON_ITEM
#undef COMMON_VALUE

#endif /*__COMMON_NODENAMES_H__*/
