#version 120
#extension GL_EXT_gpu_shader4 : enable

#define final

float sqr(final float x) {
	return x * x;
}

//begin orthogonalize

#define Quaternion vec4

//----------------------------------------------------------------------------
//  The solution is based on
//  Berthold K. P. Horn (1987),
//  "Closed-form solution of absolute orientation using unit quaternions,"
//  Journal of the Optical Society of America A, 4:629-642

Quaternion matrixToQuaternion(final mat3 A) {
	
	int i, j, k, iq, ip, numPos;
	float tresh, theta, tau, t, sm, s, h, g, c, tmp;
	float b[4];
	float z[4];
	float v[16];
	float w[4];
	float a[16];

	// on-diagonal elements
	a[0*4+0] =  A[0][0]+A[1][1]+A[2][2];
	a[1*4+1] =  A[0][0]-A[1][1]-A[2][2];
	a[2*4+2] = -A[0][0]+A[1][1]-A[2][2];
	a[3*4+3] = -A[0][0]-A[1][1]+A[2][2];

	// off-diagonal elements
	a[0*4+1] = A[2][1]-A[1][2];
	a[0*4+2] = A[0][2]-A[2][0];
	a[0*4+3] = A[1][0]-A[0][1];
	
	a[1*4+0] = A[2][1]-A[1][2];
	a[2*4+0] = A[0][2]-A[2][0];
	a[3*4+0] = A[1][0]-A[0][1];

	a[1*4+2] = A[1][0]+A[0][1];
	a[1*4+3] = A[0][2]+A[2][0];
	a[2*4+3] = A[2][1]+A[1][2];

	a[2*4+1] = A[1][0]+A[0][1];
	a[3*4+1] = A[0][2]+A[2][0];
	a[3*4+2] = A[2][1]+A[1][2];

	//jacobi iteration to find the largest eigenvalue
	
	// initialize
	for (ip=0; ip<4; ip++) 
	{
		for (iq=0; iq<4; iq++)
		{
			v[ip*4+iq] = 0.0;
		}
		v[ip*4+ip] = 1.0;
	}
	for (ip=0; ip<4; ip++) 
	{
		b[ip] = a[ip*4+ip];
		w[ip] = a[ip*4+ip];
		z[ip] = 0.0;
	}

	// begin rotation sequence
	final int maxRotations = 5;
	for (i=0; i< maxRotations; i++) 
	{
		sm = 0.0;
		for (ip=0; ip<4-1; ip++) 
		{
			for (iq=ip+1; iq<4; iq++)
			{
				sm += abs(a[ip*4+iq]);
			}
		}

		if (i < 3)                                // first 3 sweeps
		{
			tresh = 0.2*sm/(4*4);
		}
		else
		{
			tresh = 0.0;
		}

		for (ip=0; ip<3; ip++) 
		{
			for (iq=ip+1; iq<4; iq++) 
			{
				g = 100.0*abs(a[ip*4+iq]);

				if ( abs(a[ip*4+iq]) > tresh) 
				{
					h = w[iq] - w[ip];

					theta = 0.5*h / (a[ip*4+iq]);
					t = 1.0 / (abs(theta)+sqrt(1.0+theta*theta));
					if (theta < 0.0)
					{
						t = -t;
					}
					c = 1.0 / sqrt(1+t*t);
					s = t*c;
					tau = s/(1.0+c);
					h = t*a[ip*4+iq];
					z[ip] -= h;
					z[iq] += h;
					w[ip] -= h;
					w[iq] += h;
					a[ip*4+iq]=0.0;

					// ip already shifted left by 1 unit
					for (j = 0;j <= ip-1;j++) 
					{
						g=a[j*4+ip];
						h=a[j*4+iq];
						a[j*4+ip]=g-s*(h+g*tau);
						a[j*4+iq]=h+s*(g-h*tau);
					}
					// ip and iq already shifted left by 1 unit
					for (j = ip+1;j <= iq-1;j++) 
					{
						g=a[ip*4+j];
						h=a[j*4+iq];
						a[ip*4+j]=g-s*(h+g*tau);
						a[j*4+iq]=h+s*(g-h*tau);
					}
					// iq already shifted left by 1 unit
					for (j=iq+1; j<4; j++) 
					{
						g=a[ip*4+j];
						h=a[iq*4+j];
						a[ip*4+j]=g-s*(h+g*tau);
						a[iq*4+j]=h+s*(g-h*tau);
					}
					for (j=0; j<4; j++) 
					{
						g=v[j*4+ip];
						h=v[j*4+iq];
						v[j*4+ip]=g-s*(h+g*tau);
						v[j*4+iq]=h+s*(g-h*tau);
					}
				}
			}

		}

		for (ip=0; ip<4; ip++) 
		{
			b[ip] += z[ip];
			w[ip] = b[ip];
			z[ip] = 0.0;
		}
	}
	
	//select the principal eigenvector
	float largest = -100.0;
	
	int principal_index = 0;
	
	for(j=0; j < 4; j++){
		if(w[j] > largest) {
			largest = w[j];
			principal_index = j;
		}
	}
	
	return Quaternion(
		v[4 * 0 + principal_index],
		v[4 * 1 + principal_index],
		v[4 * 2 + principal_index],
		v[4 * 3 + principal_index]);
}

mat3 quaternionToMatrix(final Quaternion quaternion) {
	float ww = quaternion[0] * quaternion[0];
	float wx = quaternion[0] * quaternion[1];
	float wy = quaternion[0] * quaternion[2];
	float wz = quaternion[0] * quaternion[3];

	float xx = quaternion[1] * quaternion[1];
	float yy = quaternion[2] * quaternion[2];
	float zz = quaternion[3] * quaternion[3];

	float xy = quaternion[1] * quaternion[2];
	float xz = quaternion[1] * quaternion[3];
	float yz = quaternion[2] * quaternion[3];

	float rr = xx + yy + zz;
	// normalization factor, just in case quaternion was not normalized
	float f = float(1)/float(sqrt(ww + rr));
	float s = (ww - rr)*f;
	f *= 2;

	return mat3(
		xx * f + s,
		(xy - wz) * f,
		(xz + wy) * f,

		(xy + wz) * f,
		yy * f + s,
		(yz - wx) * f,
		
		(xz - wy) * f,
		(yz + wx) * f,
		zz*f + s);
}

// From http://cache-www.intel.com/cd/00/00/29/37/293748_293748.pdf

Quaternion matrixToQuaternionIntel(final mat3 m) {

	Quaternion q;
	float m00 = m[0].x;
	float m01 = m[0].y;
	float m02 = m[0].z;
	float m10 = m[1].x;
	float m11 = m[1].y;
	float m12 = m[1].z;
	float m20 = m[2].x;
	float m21 = m[2].y;
	float m22 = m[2].z;

	if ( m00 + m11 + m22 > 0.0 ) {
		float t = + m00 + m11 + m22 + 1.0;
		float s = 0.5 / sqrt(t);
		q[3] = s * t;
		q[2] = ( m01 - m10 ) * s;
		q[1] = ( m20 - m02 ) * s;
		q[0] = ( m12 - m21 ) * s;
	} else if ( m00 > m11 && m00 > m22 ) {
		float t = + m00 - m11 - m22 + 1.0;
		float s = 0.5 / sqrt(t);
		q[0] = s * t;
		q[2] = ( m20 + m02 ) * s;
		q[1] = ( m01 + m10 ) * s;
		q[3] = ( m12 - m21 ) * s;
	} else if ( m11 > m22 ) {
		float t = - m00 + m11 - m22 + 1.0;
		float s = 0.5 / sqrt(t);
		q[1] = s * t;
		q[0] = ( m01 + m10 ) * s;
		q[3] = ( m20 - m02 ) * s;
		q[2] = ( m12 + m21 ) * s;
	} else {
		float t = - m00 - m11 + m22 + 1.0;
		float s = 0.5 / sqrt(t);
		q[2] = s * t;
		q[3] = ( m01 - m10 ) * s;
		q[0] = ( m20 + m02 ) * s;
		q[1] = ( m12 + m21 ) * s;
	}

	return q;
}

float getComponentQuaternion(Quaternion q, final int index) {
	if(index == 0) return q.x;
	if(index == 1) return q.y;
	if(index == 2) return q.z;
	if(index == 3) return q.w;
}

float getComponentVec3(vec3 v, final int index) {
	if(index == 0) return v.x;
	if(index == 1) return v.y;
	if(index == 2) return v.z;
}

vec3 setComponentVec3(vec3 v, float f, final int index) {
	if(index == 0) v.x = f;
	if(index == 1) v.y = f;
	if(index == 2) v.z = f;
	return v;
}

Quaternion setComponentQuaternion(Quaternion q, float f, final int index) {
	if(index == 0) q.x = f;
	if(index == 1) q.y = f;
	if(index == 2) q.z = f;
	if(index == 3) q.w = f;
	return q;
}

// From http://www.melax.com/diag.html

Quaternion matrixToQuaternionMelax(final mat3 A) {
	// A must be a symmetric matrix.
	// returns quaternion q such that its corresponding matrix Q 
	// can be used to Diagonalize A
	// Diagonal matrix D = Q * A * Transpose(Q);  and  A = QT*D*Q
	// The rows of q are the eigenvectors D's diagonal is the eigenvalues
	// As per 'row' convention if mat3 Q = q.getmatrix(); then v*Q = q*v*conj(q)
	const int maxsteps = 7;  // certainly wont need that many.
	Quaternion q = Quaternion(0,0,0,1);
	for(int i = 0; i < maxsteps; i++) {
		
		final mat3 Q = quaternionToMatrix(q);
		//mat3 Q  = q.getmatrix(); // v*Q == q*v*conj(q)
		
		final mat3 D  = Q * A * transpose(Q);  // A = Q^T*D*Q
		
		//vec3 offdiag(D[1][2],D[0][2],D[0][1]); // elements not on the diagonal
		//vec3 offdiag = vec3(D[2].y,D[2].x,D[1].x); // elements not on the diagonal
		vec3 offdiag = vec3(D[1].z, D[0].z, D[0].y); // elements not on the diagonal

		
		vec3 om = vec3(abs(offdiag.x),abs(offdiag.y),abs(offdiag.z)); // mag of each offdiag elem
		
		int k = (om.x > om.y && om.x > om.z)?0: (om.y > om.z)? 1 : 2; // index of largest element of offdiag
		
		int k1 = (k+1)%3;
		int k2 = (k+2)%3;
		if(getComponentVec3(offdiag, k)==0.0f) break;  // diagonal already

		//float thet = (D[k2][k2]-D[k1][k1])/(2.0f*offdiag[k]);
		float thet = (getComponentVec3(D[k2],k2)-getComponentVec3(D[k1],k1))/(2.0f*getComponentVec3(offdiag, k));

		
		float sgn = (thet > 0.0f)?1.0f:-1.0f;		
		
		thet    *= sgn; // make it positive
		
		float t = sgn /(thet +((thet < 1.E6f)?sqrt(sqr(thet)+1.0f):thet)) ; // sign(T)/(|T|+sqrt(T^2+1))
		
		
		float c = 1.0f/sqrt(sqr(t)+1.0f); //  c= 1/(t^2+1) , t=s/c 
		

		if(c==1.0f) break;  // no room for improvement - reached machine precision.

		Quaternion jr = Quaternion(0,0,0,0); // jacobi rotation for this iteration.

		
		//jr[k] = sgn*sqrt((1.0f-c)/2.0f);  // using 1/2 angle identity sin(a/2) = sqrt((1-cos(a))/2)  		
		//jr[k] *= -1.0f; // since our quat-to-matrix convention was for v*M instead of M*v

		jr = setComponentQuaternion(jr, -sgn*sqrt((1.0f-c)/2.0f), k);
		
		//jr.w  = sqrt(1.0f - sqr(jr[k]));
		jr.w  = sqrt(1.0f - sqr(getComponentQuaternion(jr, k)));
		
		if(jr.w==1.0f) break; // reached limits of floating point precision

		q =  q*jr;  
		q = normalize(q);
	} 
	return q;
}

mat3 orthogonalize(final mat3 a) {
	
	// Do orthogonalization using a quaternion intermediate
	// (this, essentially, does the orthogonalization via
	// diagonalization of an appropriately finalructed symmetric
	// 4x4 matrix rather than by doing SVD of the 3x3 matrix)
	
	return transpose(quaternionToMatrix(matrixToQuaternion(transpose(a)))); 
}


//end orthogonalize


const int maxAnchorCount = 80;

// A single anchor
struct Anchor {
	float target_x;
	float target_y;
	float constraint_x;
	float constraint_y;
};

struct Edit {
	int anchorCount;
	Anchor anchors[maxAnchorCount];
};

varying vec2 texture_coordinate; 
uniform Edit edit;
uniform sampler2D my_color_texture; 
uniform int mapping;

void main()
{
    vec3 average_constraint = vec3(0.0,0.0,0.0);
    vec3 average_target = vec3(0.0,0.0,0.0);
    float constraintsWeightSum = 0.0;
    
    mat3 rotSum = mat3(0.0001, 0.0, 0.0, 
                    0.0, 0.0001, 0.0,
                    0.0, 0.0, 0.0001);
    
    if(edit.anchorCount > 0) 
    {
        vec3 current = vec3(texture_coordinate.x, texture_coordinate.y, 0.0);
        for(int j = 0; j < edit.anchorCount; j++) 
        {
	        vec3 target = vec3(edit.anchors[j].target_x, edit.anchors[j].target_y, 0.0);
	        vec3 constraint = vec3(edit.anchors[j].constraint_x, edit.anchors[j].constraint_y, 0.0);
		    float constraintWeight = 1.0 / pow(length(current - target), 2.0);
		
		    constraintsWeightSum += constraintWeight;
		    average_constraint += constraintWeight * constraint;
		    average_target += constraintWeight * target;
	    }
        average_constraint /= constraintsWeightSum;	
        average_target /= constraintsWeightSum;
    
        for(int j = 0; j < edit.anchorCount; j++) 
        {
	        vec3 target = vec3(edit.anchors[j].target_x, edit.anchors[j].target_y, 0.0);
	        vec3 constraint = vec3(edit.anchors[j].constraint_x, edit.anchors[j].constraint_y, 0.0);
		    float constraintWeight = 1.0 / pow(length(current - target), 2.0);
		    vec3 sc = constraint - average_constraint;
            vec3 st = target - average_target;
            
            mat3 rot = mat3(st.x*sc.x, st.x*sc.y, st.x*sc.z, 
                            st.y*sc.x, st.y*sc.y, st.y*sc.z,
                            st.z*sc.x, st.z*sc.y, st.z*sc.z);
            
		    rotSum += constraintWeight * rot;
	    }
	    
        
    }
    
    rotSum = orthogonalize(rotSum);
    
    vec3 offset = vec3(texture_coordinate.x, texture_coordinate.y, 0.0) - average_target;
    offset = rotSum * offset;
    offset += average_constraint;
    
    
    vec2 new_texture_coordinate = vec2(offset.x , offset.y);
    if(mapping == 0)
    {
				gl_FragColor = texture2D(my_color_texture, new_texture_coordinate);
		}else {
				gl_FragColor = vec4(mod(new_texture_coordinate.x, 1.0),mod(new_texture_coordinate.y, 1.0), 0.0, 1.0);
		}
				
}
