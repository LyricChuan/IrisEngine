#pragma once

struct fvector_3d;
struct frotator;

__declspec(align(16)) struct fquat
{
public:
	float x;
	float y;
	float z;
	float w;

public:
	fquat();
	fquat(float in_x, float in_y, float in_z, float in_w);

	fquat operator*(const fquat& q) const;
	fquat operator*=(const fquat& q) ;
	fquat operator*(const float& q) const;
	fquat operator*(const int& q) const;
	fquat operator*=(const float& q);
	fquat operator*=(const int& q);
	fquat operator+(const fquat& q) const;
	fquat operator-(const fquat& q) const;
	fquat operator+=(const fquat& q);
	fquat operator-=(const fquat& q);
	float operator|(const fquat& q)const;

	fquat log()const;
	fquat exp()const;

	float size() const;
	float size_squared() const;

	float get_half_angle_radian() const;
	float get_angle_radian() const;
	float get_angle()const;

	void normalize(float in_tolerance = 1.e-8f);

	bool is_normalized() const;

	void rotator_by_x(float theta);
	void rotator_by_y(float theta);
	void rotator_by_z(float theta);
	void rotator_by_axis(float theta, const fvector_3d& axis);

	fquat inverse()const;

	//欧拉转四元
	//惯性->物体
	void inertia_to_object(const frotator& in_rot);

	//物体->惯性
	void object_to_inertia(const frotator& in_rot);

	//没有归一化的fquat
	static fquat lerp(const fquat &in_q0,const fquat &in_q1,float in_t);

	//双线性四元数插值
	static fquat bilinear_lerp(
		const fquat& in_q00, const fquat& in_q10,
		const fquat& in_q01, const fquat& in_q11,
		float frac_x, float frac_y);
	
	//没有归一化
	static fquat s_lerp(const fquat& in_q0, const fquat& in_q1, float in_t);

	//四元数插值 全路径
	static fquat s_lerp_full_path(const fquat& in_q0, const fquat& in_q1, float in_t);
	static fquat s_quad(const fquat& in_q0, const fquat& in_q1, const fquat& in_s0, const fquat& in_s1, float in_t);
	static fquat s_quad_full_path(const fquat& in_q0, const fquat& in_q1, const fquat& in_s0, const fquat& in_s1, float in_t);
	
	//获取in_s0 in_s1
	static fquat get_tangents(const fquat& in_prev_q, const fquat& in_q, const fquat& in_nest_q);
	
	static const fquat identity;
};