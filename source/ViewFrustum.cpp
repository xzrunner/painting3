// from "Focus On 3D Terrain Programming"

#include "painting3/ViewFrustum.h"

#include <painting0/Camera.h>

#include <string.h>

namespace
{

const int FRUSTUM_RIGHT  = 0;
const int FRUSTUM_LEFT   = 1;
const int FRUSTUM_BOTTOM = 2;
const int FRUSTUM_TOP    = 3;
const int FRUSTUM_FAR    = 4;
const int FRUSTUM_NEAR   = 5;

}

namespace pt3
{

ViewFrustum::ViewFrustum()
{
	memset(m_view_frustum, 0, sizeof(m_view_frustum));
}

void ViewFrustum::CalculateViewFrustum(const pt0::Camera& cam) const
{
	auto proj_mat = cam.GetProjectionMat();
	auto proj_ptr = proj_mat.x;

	auto mv_mat = cam.GetModelViewMat();
	auto mv_ptr = mv_mat.x;

	float clip[16];
	float norm;

	/* Combine the two matrices (multiply projection by modelview) */
	clip[0]= mv_ptr[0]*proj_ptr[0] + mv_ptr[1]*proj_ptr[4] + mv_ptr[2]*proj_ptr[8] + mv_ptr[3]*proj_ptr[12];
	clip[1]= mv_ptr[0]*proj_ptr[1] + mv_ptr[1]*proj_ptr[5] + mv_ptr[2]*proj_ptr[9] + mv_ptr[3]*proj_ptr[13];
	clip[2]= mv_ptr[0]*proj_ptr[2] + mv_ptr[1]*proj_ptr[6] + mv_ptr[2]*proj_ptr[10] + mv_ptr[3]*proj_ptr[14];
	clip[3]= mv_ptr[0]*proj_ptr[3] + mv_ptr[1]*proj_ptr[7] + mv_ptr[2]*proj_ptr[11] + mv_ptr[3]*proj_ptr[15];

	clip[4]= mv_ptr[4]*proj_ptr[0] + mv_ptr[5]*proj_ptr[4] + mv_ptr[6]*proj_ptr[8] + mv_ptr[7]*proj_ptr[12];
	clip[5]= mv_ptr[4]*proj_ptr[1] + mv_ptr[5]*proj_ptr[5] + mv_ptr[6]*proj_ptr[9] + mv_ptr[7]*proj_ptr[13];
	clip[6]= mv_ptr[4]*proj_ptr[2] + mv_ptr[5]*proj_ptr[6] + mv_ptr[6]*proj_ptr[10] + mv_ptr[7]*proj_ptr[14];
	clip[7]= mv_ptr[4]*proj_ptr[3] + mv_ptr[5]*proj_ptr[7] + mv_ptr[6]*proj_ptr[11] + mv_ptr[7]*proj_ptr[15];

	clip[8]= mv_ptr[8]*proj_ptr[0] + mv_ptr[9]*proj_ptr[4] + mv_ptr[10]*proj_ptr[8] + mv_ptr[11]*proj_ptr[12];
	clip[9]= mv_ptr[8]*proj_ptr[1] + mv_ptr[9]*proj_ptr[5] + mv_ptr[10]*proj_ptr[9] + mv_ptr[11]*proj_ptr[13];
	clip[10]= mv_ptr[8]*proj_ptr[2] + mv_ptr[9]*proj_ptr[6] + mv_ptr[10]*proj_ptr[10] + mv_ptr[11]*proj_ptr[14];
	clip[11]= mv_ptr[8]*proj_ptr[3] + mv_ptr[9]*proj_ptr[7] + mv_ptr[10]*proj_ptr[11] + mv_ptr[11]*proj_ptr[15];

	clip[12]= mv_ptr[12]*proj_ptr[0] + mv_ptr[13]*proj_ptr[4] + mv_ptr[14]*proj_ptr[8] + mv_ptr[15]*proj_ptr[12];
	clip[13]= mv_ptr[12]*proj_ptr[1] + mv_ptr[13]*proj_ptr[5] + mv_ptr[14]*proj_ptr[9] + mv_ptr[15]*proj_ptr[13];
	clip[14]= mv_ptr[12]*proj_ptr[2] + mv_ptr[13]*proj_ptr[6] + mv_ptr[14]*proj_ptr[10] + mv_ptr[15]*proj_ptr[14];
	clip[15]= mv_ptr[12]*proj_ptr[3] + mv_ptr[13]*proj_ptr[7] + mv_ptr[14]*proj_ptr[11] + mv_ptr[15]*proj_ptr[15];

	/* Extract the numbers for the RIGHT plane */
	m_view_frustum[FRUSTUM_RIGHT][0]= clip[3] - clip[0];
	m_view_frustum[FRUSTUM_RIGHT][1]= clip[7] - clip[4];
	m_view_frustum[FRUSTUM_RIGHT][2]= clip[11] - clip[8];
	m_view_frustum[FRUSTUM_RIGHT][3]= clip[15] - clip[12];

	//normalize the plane
	norm= sqrtf(std::pow(m_view_frustum[FRUSTUM_RIGHT][0], 2)+
				 std::pow(m_view_frustum[FRUSTUM_RIGHT][1], 2)+
				 std::pow(m_view_frustum[FRUSTUM_RIGHT][2], 2));
	m_view_frustum[FRUSTUM_RIGHT][0]/= norm;
	m_view_frustum[FRUSTUM_RIGHT][1]/= norm;
	m_view_frustum[FRUSTUM_RIGHT][2]/= norm;
	m_view_frustum[FRUSTUM_RIGHT][3]/= norm;

	/* Extract the numbers for the LEFT plane */
	m_view_frustum[FRUSTUM_LEFT][0]= clip[3] + clip[0];
	m_view_frustum[FRUSTUM_LEFT][1]= clip[7] + clip[4];
	m_view_frustum[FRUSTUM_LEFT][2]= clip[11] + clip[8];
	m_view_frustum[FRUSTUM_LEFT][3]= clip[15] + clip[12];

	//normalize the plane
	norm= sqrtf(std::pow(m_view_frustum[FRUSTUM_LEFT][0], 2)+
				 std::pow(m_view_frustum[FRUSTUM_LEFT][1], 2)+
				 std::pow(m_view_frustum[FRUSTUM_LEFT][2], 2));
	m_view_frustum[FRUSTUM_LEFT][0]/= norm;
	m_view_frustum[FRUSTUM_LEFT][1]/= norm;
	m_view_frustum[FRUSTUM_LEFT][2]/= norm;
	m_view_frustum[FRUSTUM_LEFT][3]/= norm;

	/* Extract the BOTTOM plane */
	m_view_frustum[FRUSTUM_BOTTOM][0]= clip[3] + clip[1];
	m_view_frustum[FRUSTUM_BOTTOM][1]= clip[7] + clip[5];
	m_view_frustum[FRUSTUM_BOTTOM][2]= clip[11] + clip[9];
	m_view_frustum[FRUSTUM_BOTTOM][3]= clip[15] + clip[13];

	//normalize the plane
	norm= sqrtf(std::pow(m_view_frustum[FRUSTUM_BOTTOM][0], 2)+
				 std::pow(m_view_frustum[FRUSTUM_BOTTOM][1], 2)+
				 std::pow(m_view_frustum[FRUSTUM_BOTTOM][2], 2));
	m_view_frustum[FRUSTUM_BOTTOM][0]/= norm;
	m_view_frustum[FRUSTUM_BOTTOM][1]/= norm;
	m_view_frustum[FRUSTUM_BOTTOM][2]/= norm;
	m_view_frustum[FRUSTUM_BOTTOM][3]/= norm;

	/* Extract the TOP plane */
	m_view_frustum[FRUSTUM_TOP][0]= clip[3] - clip[1];
	m_view_frustum[FRUSTUM_TOP][1]= clip[7] - clip[5];
	m_view_frustum[FRUSTUM_TOP][2]= clip[11] - clip[9];
	m_view_frustum[FRUSTUM_TOP][3]= clip[15] - clip[13];

	//normalize the plane
	norm= sqrtf(std::pow(m_view_frustum[FRUSTUM_TOP][0], 2)+
				 std::pow(m_view_frustum[FRUSTUM_TOP][1], 2)+
				 std::pow(m_view_frustum[FRUSTUM_TOP][2], 2));
	m_view_frustum[FRUSTUM_TOP][0]/= norm;
	m_view_frustum[FRUSTUM_TOP][1]/= norm;
	m_view_frustum[FRUSTUM_TOP][2]/= norm;
	m_view_frustum[FRUSTUM_TOP][3]/= norm;

	/* Extract the FAR plane */
	m_view_frustum[FRUSTUM_FAR][0]= clip[3] - clip[2];
	m_view_frustum[FRUSTUM_FAR][1]= clip[7] - clip[6];
	m_view_frustum[FRUSTUM_FAR][2]= clip[11] - clip[10];
	m_view_frustum[FRUSTUM_FAR][3]= clip[15] - clip[14];

	//normalize the plane
	norm= sqrtf(std::pow(m_view_frustum[FRUSTUM_FAR][0], 2)+
				 std::pow(m_view_frustum[FRUSTUM_FAR][1], 2)+
				 std::pow(m_view_frustum[FRUSTUM_FAR][2], 2));
	m_view_frustum[FRUSTUM_FAR][0]/= norm;
	m_view_frustum[FRUSTUM_FAR][1]/= norm;
	m_view_frustum[FRUSTUM_FAR][2]/= norm;
	m_view_frustum[FRUSTUM_FAR][3]/= norm;

	/* Extract the NEAR plane */
	m_view_frustum[FRUSTUM_NEAR][0]= clip[3] + clip[2];
	m_view_frustum[FRUSTUM_NEAR][1]= clip[7] + clip[6];
	m_view_frustum[FRUSTUM_NEAR][2]= clip[11] + clip[10];
	m_view_frustum[FRUSTUM_NEAR][3]= clip[15] + clip[14];

	//normalize the plane
	norm= sqrtf(std::pow(m_view_frustum[FRUSTUM_NEAR][0], 2)+
				 std::pow(m_view_frustum[FRUSTUM_NEAR][1], 2)+
				 std::pow(m_view_frustum[FRUSTUM_NEAR][2], 2));
	m_view_frustum[FRUSTUM_NEAR][0]/= norm;
	m_view_frustum[FRUSTUM_NEAR][1]/= norm;
	m_view_frustum[FRUSTUM_NEAR][2]/= norm;
	m_view_frustum[FRUSTUM_NEAR][3]/= norm;
}

bool ViewFrustum::VertexFrustumTest(float x, float y, float z,
	                                bool test_lr, bool test_tb, bool test_nf) const
{
	int i;

	//loop through the frustum planes
	for( i=0; i<6; i++ )
	{
		if( ( i==FRUSTUM_RIGHT || i==FRUSTUM_LEFT ) && test_lr)
		{
			if( m_view_frustum[i][0]*x + m_view_frustum[i][1]*y + m_view_frustum[i][2]*z + m_view_frustum[i][3] <= 0 )
				return false;
		}

		if( ( i==FRUSTUM_TOP || i==FRUSTUM_BOTTOM ) && test_tb)
		{
			if( m_view_frustum[i][0]*x + m_view_frustum[i][1]*y + m_view_frustum[i][2]*z + m_view_frustum[i][3] <= 0 )
				return false;
		}

		if( ( i==FRUSTUM_NEAR || i==FRUSTUM_FAR ) && test_nf)
		{
			if( m_view_frustum[i][0]*x + m_view_frustum[i][1]*y + m_view_frustum[i][2]*z + m_view_frustum[i][3] <= 0 )
				return false;
		}
	}

	return true;
}

bool ViewFrustum::CubeFrustumTest(float x, float y, float z, float size) const
{
	int i;

	//loop through and test the six vertices of the bounding box against the viewing frustum
	for( i=0; i<6; i++ )
	{
		if( m_view_frustum[i][0] * ( x-size )+m_view_frustum[i][1] *
								  ( y-size )+m_view_frustum[i][2] *
								  ( z-size )+m_view_frustum[i][3] > 0 )
			continue;
		if( m_view_frustum[i][0] * ( x+size )+m_view_frustum[i][1] *
								  ( y-size )+m_view_frustum[i][2] *
								  ( z-size )+m_view_frustum[i][3] > 0 )
			continue;
		if( m_view_frustum[i][0] * ( x-size )+m_view_frustum[i][1] *
								  ( y+size )+m_view_frustum[i][2] *
								  ( z-size )+m_view_frustum[i][3] > 0 )
			continue;
		if( m_view_frustum[i][0] * ( x+size )+m_view_frustum[i][1] *
								  ( y+size )+m_view_frustum[i][2] *
								  ( z-size )+m_view_frustum[i][3] > 0 )
			continue;
		if( m_view_frustum[i][0] * ( x-size )+m_view_frustum[i][1] *
								  ( y-size )+m_view_frustum[i][2] *
								  ( z+size )+m_view_frustum[i][3] > 0 )
			continue;
		if( m_view_frustum[i][0] * ( x+size )+m_view_frustum[i][1] *
								  ( y-size )+m_view_frustum[i][2] *
								  ( z+size )+m_view_frustum[i][3] > 0 )
			continue;
		if( m_view_frustum[i][0] * ( x-size )+m_view_frustum[i][1] *
								  ( y+size )+m_view_frustum[i][2] *
								  ( z+size )+m_view_frustum[i][3] > 0 )
			continue;
		if( m_view_frustum[i][0] * ( x+size )+m_view_frustum[i][1] *
								  ( y+size )+m_view_frustum[i][2] *
								  ( z+size )+m_view_frustum[i][3] > 0 )
			continue;

		return false;
	}

	return true;
}

bool ViewFrustum::SphereInFrustum(float x, float y, float z, float radius) const
{
	int i;

	for( i=0; i<6; i++ )
	{
		if( m_view_frustum[i][0] * x +
			m_view_frustum[i][1] * y +
			m_view_frustum[i][2] * z +
			m_view_frustum[i][3] < -radius)
			return false;
	}

	return true;
}

}