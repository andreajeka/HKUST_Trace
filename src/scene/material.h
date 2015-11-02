//
// material.h
//
// The Material class: a description of the phsyical properties of a surface
// that are used to determine how that surface interacts with light.

#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "../vecmath/vecmath.h"
#include <string>

class Scene;
class ray;
class isect;

using std::string;

/*
MaterialParameter is a helper class for a material;
it stores either a constant value (in a 3-vector)
or else a link to a map of some type.  If the pointer
to the TextureMap is NULL, the class simply returns
whatever is stored in the constant vector.

This is unabashedly a speed hack; we are replacing a
potential use of polymorphism with an old-school "if"
statement.  But raytracers are slow enough, and the
Material class is used often enough, that it is
(somewhat) justifiable to do this.
*/

class MaterialParameter
{
public:
	explicit MaterialParameter(const vec3f& par)
		: _value(par)
	{ }

	explicit MaterialParameter(const double par)
		: _value(par, par, par)
	{ }

	MaterialParameter()
		: _value(0.0, 0.0, 0.0)
	{ }

	MaterialParameter& operator*=(const MaterialParameter& rhs)
	{
		(*this) *= rhs._value;
		return *this;
	}

	vec3f& operator*=(const vec3f& rhs)
	{
		_value[0] *= rhs[0];
		_value[1] *= rhs[1];
		_value[2] *= rhs[2];
		return _value;
	}

	vec3f& operator*=(const double rhs)
	{
		_value[0] *= rhs;
		_value[1] *= rhs;
		_value[2] *= rhs;
		return _value;
	}

	MaterialParameter& operator+=(const MaterialParameter& rhs)
	{
		_value += rhs._value;
		return *this;
	}

	void setValue(const vec3f& rhs)
	{
		_value = rhs;
	}

	void setValue(const double rhs)
	{
		_value[0] = rhs;
		_value[1] = rhs;
		_value[2] = rhs;
	}

	bool isZero() { return _value.iszero(); }

	vec3f& operator+=(const vec3f& rhs)
	{
		_value += rhs;
		return _value;
	}

	vec3f value(const isect& is) const;
	double intensityValue(const isect& is) const;

private:
	vec3f _value;
};

class Material
{

public:
	Material()
		: _ke(vec3f(0.0, 0.0, 0.0))
		, _ka(vec3f(0.0, 0.0, 0.0))
		, _ks(vec3f(0.0, 0.0, 0.0))
		, _kd(vec3f(0.0, 0.0, 0.0))
		, _kr(vec3f(0.0, 0.0, 0.0))
		, _kt(vec3f(0.0, 0.0, 0.0))
		, _refl(0)
		, _trans(0)
		, _shininess(0.0)
		, _index(1.0) {}

	Material(const vec3f& e, const vec3f& a, const vec3f& s,
		const vec3f& d, const vec3f& r, const vec3f& t, double sh, double in)
		: _ke(e), _ka(a), _ks(s), _kd(d), _kr(r), _kt(t),
		_shininess(vec3f(sh, sh, sh)), _index(vec3f(in, in, in)) {
		setBools();
	}

	virtual vec3f shade(Scene *scene, const ray& r, const isect& i) const;



	Material &
		operator+=(const Material &m)
	{
		_ke += m._ke;
		_ka += m._ka;
		_ks += m._ks;
		_kd += m._kd;
		_kr += m._kr;
		_kt += m._kt;
		_index += m._index;
		_shininess += m._shininess;
		return *this;
	}

	friend Material operator*(double d, Material m);

	// Accessor functions; we pass in an isect& for cases where
	// the parameter is dependent on, for example, world-space
	// coordinates (i.e., solid textures) or parametrized coordinates
	// (i.e., mapped textures)
	vec3f ke(const isect& i) const { return _ke.value(i); }
	vec3f ka(const isect& i) const { return _ka.value(i); }
	vec3f ks(const isect& i) const { return _ks.value(i); }
	vec3f kd(const isect& i) const { return _kd.value(i); }
	vec3f kr(const isect& i) const { return _kr.value(i); }
	vec3f kt(const isect& i) const { return _kt.value(i); }
	double shininess(const isect& i) const
	{
		// Have to renormalize into the range 0-128 if it's texture mapped.
		return _shininess.intensityValue(i);
	}

	double index(const isect& i) const { return _index.intensityValue(i); }

	// setting functions accepting primitives (vec3f and double)
	void setEmissive(const vec3f& ke)     { _ke.setValue(ke); }
	void setAmbient(const vec3f& ka)      { _ka.setValue(ka); }
	void setSpecular(const vec3f& ks)     { _ks.setValue(ks); setBools(); }
	void setDiffuse(const vec3f& kd)      { _kd.setValue(kd); }
	void setReflective(const vec3f& kr)   { _kr.setValue(kr); setBools(); }
	void setTransmissive(const vec3f& kt) { _kt.setValue(kt); setBools(); }
	void setShininess(double shininess)
	{
		_shininess.setValue(shininess);
	}
	void setIndex(double index)           { _index.setValue(index); }


	// setting functions taking MaterialParameters
	void setEmissive(const MaterialParameter& ke)            { _ke = ke; }
	void setAmbient(const MaterialParameter& ka)             { _ka = ka; }
	void setSpecular(const MaterialParameter& ks)            { _ks = ks; }
	void setDiffuse(const MaterialParameter& kd)             { _kd = kd; }
	void setReflective(const MaterialParameter& kr)          { _kr = kr; setBools(); }
	void setTransmissive(const MaterialParameter& kt)        { _kt = kt; setBools(); }
	void setShininess(const MaterialParameter& shininess)
	{
		_shininess = shininess;
	}
	void setIndex(const MaterialParameter& index)            { _index = index; }

	// get booleans for reflection and refraction
	bool Refl() const { return _refl; }
	bool Trans() const { return _trans; }
	bool Recur() const { return _recur; }
	bool Spec() const { return _spec; }
	bool Both() const { return _both; }

private:
	MaterialParameter _ke;                    // emissive
	MaterialParameter _ka;                    // ambient
	MaterialParameter _ks;                    // specular
	MaterialParameter _kd;                    // diffuse
	MaterialParameter _kr;                    // reflective
	MaterialParameter _kt;                    // transmissive

	bool _refl;								  // specular reflector?
	bool _trans;							  // specular transmitter?
	bool _recur;							  // either one
	bool _spec;								  // any kind of specular?
	bool _both;								  // reflection and transmission

	MaterialParameter _shininess;
	MaterialParameter _index;                 // index of refraction

	void setBools() {
		_refl = !_kr.isZero();
		_trans = !_kt.isZero();
		_recur = _refl || _trans;
		_spec = _refl || !_ks.isZero();
		_both = _refl && _trans;
	}

};

// This doesn't necessarily make sense for mapped materials
inline Material
operator*(double d, Material m)
{
	m._ke *= d;
	m._ka *= d;
	m._ks *= d;
	m._kd *= d;
	m._kr *= d;
	m._kt *= d;
	m._index *= d;
	m._shininess *= d;
	return m;
}

#endif // __MATERIAL_H__
