@insertpiece( SetCrossPlatformSettings )
@insertpiece( DeclUavCrossPlatform )

#define ushort2 uint2

@insertpiece( PreBindingsHeaderCS )

Buffer<float2> integrationTapsBuffer	: register(t0);

RWTexture2D<@insertpiece(uav0_pf_type)> irradianceField;

@property( integrate_depth )
	groupshared float2 g_sharedValues[@value( threads_per_group_x ) * @value( threads_per_group_y )];
@else
	groupshared float4 g_sharedValues[@value( threads_per_group_x ) * @value( threads_per_group_y )];
@end

@insertpiece( HeaderCS )

//in uvec3 gl_NumWorkGroups;
//in uvec3 gl_WorkGroupID;
//in uvec3 gl_LocalInvocationID;
//in uvec3 gl_GlobalInvocationID;
//in uint  gl_LocalInvocationIndex;

[numthreads(@value( threads_per_group_x ), @value( threads_per_group_y ), @value( threads_per_group_z ))]
void main
(
	uint3 gl_WorkGroupID			: SV_GroupID,
	uint3 gl_LocalInvocationID		: SV_GroupThreadID,
	uint gl_LocalInvocationIndex	: SV_GroupIndex
)
{
	@insertpiece( BodyCS )
}
