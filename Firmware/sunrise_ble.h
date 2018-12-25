#ifndef SUNRISE_BLE_H
#define SUNRISE_BLE_H

#include <stdbool.h>

enum SUNRISE_BLE_UUID_t {
	SUNRISE_BLE_UUID_SERVICE = 0x1000,
	SUNRISE_BLE_UUID_CLOCK,
	SUNRISE_BLE_UUID_LATITUDE,
	SUNRISE_BLE_UUID_LONGITUDE,
	SUNRISE_BLE_UUID_DAY_TEMPERATURE,
	SUNRISE_BLE_UUID_NIGHT_TEMPERATURE,
	SUNRISE_BLE_UUID_DAY_BRIGHTNESS,
	SUNRISE_BLE_UUID_NIGHT_BRIGHTNESS,
};


enum BLE_UNIT_t {
	BLE_UNIT_UNITLESS                                               = 0x2700, // unitless
	BLE_UNIT_LENGTH_METRE                                           = 0x2701, // length (metre)
	BLE_UNIT_MASS_KILOGRAM                                          = 0x2702, // mass (kilogram)
	BLE_UNIT_TIME_SECOND                                            = 0x2703, // time (second)
	BLE_UNIT_ELECTRIC_CURRENT_AMPERE                                = 0x2704, // electric current (ampere)
	BLE_UNIT_THERMODYNAMIC_TEMPERATURE_KELVIN                       = 0x2705, // thermodynamic temperature (kelvin)
	BLE_UNIT_AMOUNT_OF_SUBSTANCE_MOLE                               = 0x2706, // amount of substance (mole)
	BLE_UNIT_LUMINOUS_INTENSITY_CANDELA                             = 0x2707, // luminous intensity (candela)

	BLE_UNIT_AREA_SQUARE_METRES                                     = 0x2710, // area (square metres)
	BLE_UNIT_VOLUME_CUBIC_METRES                                    = 0x2711, // volume (cubic metres)
	BLE_UNIT_VELOCITY_METRES_PER_SECOND                             = 0x2712, // velocity (metres per second)
	BLE_UNIT_ACCELERATION_METRES_PER_SECOND_SQUARED                 = 0x2713, // acceleration (metres per second squared)
	BLE_UNIT_WAVENUMBER_RECIPROCAL_METRE                            = 0x2714, // wavenumber (reciprocal metre)
	BLE_UNIT_DENSITY_KILOGRAM_PER_CUBIC_METRE                       = 0x2715, // density (kilogram per cubic metre)
	BLE_UNIT_SURFACE_DENSITY_KILOGRAM_PER_SQUARE_METRE              = 0x2716, // surface density (kilogram per square metre)
	BLE_UNIT_SPECIFIC_VOLUME_CUBIC_METRE_PER_KILOGRAM               = 0x2717, // specific volume (cubic metre per kilogram)
	BLE_UNIT_CURRENT_DENSITY_AMPERE_PER_SQUARE_METRE                = 0x2718, // current density (ampere per square metre)
	BLE_UNIT_MAGNETIC_FIELD_STRENGTH_AMPERE_PER_METRE               = 0x2719, // magnetic field strength (ampere per metre)
	BLE_UNIT_AMOUNT_CONCENTRATION_MOLE_PER_CUBIC_METRE              = 0x271A, // amount concentration (mole per cubic metre)
	BLE_UNIT_MASS_CONCENTRATION_KILOGRAM_PER_CUBIC_METRE            = 0x271B, // mass concentration (kilogram per cubic metre)
	BLE_UNIT_LUMINANCE_CANDELA_PER_SQUARE_METRE                     = 0x271C, // luminance (candela per square metre)
	BLE_UNIT_REFRACTIVE_INDEX                                       = 0x271D, // refractive index
	BLE_UNIT_RELATIVE_PERMEABILITY                                  = 0x271E, // relative permeability

	BLE_UNIT_PLANE_ANGLE_RADIAN                                     = 0x2720, // plane angle (radian)
	BLE_UNIT_SOLID_ANGLE_STERADIAN                                  = 0x2721, // solid angle (steradian)
	BLE_UNIT_FREQUENCY_HERTZ                                        = 0x2722, // frequency (hertz)
	BLE_UNIT_FORCE_NEWTON                                           = 0x2723, // force (newton)
	BLE_UNIT_PRESSURE_PASCAL                                        = 0x2724, // pressure (pascal)
	BLE_UNIT_ENERGY_JOULE                                           = 0x2725, // energy (joule)
	BLE_UNIT_POWER_WATT                                             = 0x2726, // power (watt)
	BLE_UNIT_ELECTRIC_CHARGE_COULOMB                                = 0x2727, // electric charge (coulomb)
	BLE_UNIT_ELECTRIC_POTENTIAL_DIFFERENCE_VOLT                     = 0x2728, // electric potential difference (volt)
	BLE_UNIT_CAPACITANCE_FARAD                                      = 0x2729, // capacitance (farad)
	BLE_UNIT_ELECTRIC_RESISTANCE_OHM                                = 0x272A, // electric resistance (ohm)
	BLE_UNIT_ELECTRIC_CONDUCTANCE_SIEMENS                           = 0x272B, // electric conductance (siemens)
	BLE_UNIT_MAGNETIC_FLUX_WEBER                                    = 0x272C, // magnetic flux (weber)
	BLE_UNIT_MAGNETIC_FLUX_DENSITY_TESLA                            = 0x272D, // magnetic flux density (tesla)
	BLE_UNIT_INDUCTANCE_HENRY                                       = 0x272E, // inductance (henry)
	BLE_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_CELSIUS               = 0x272F, // Celsius temperature (degree Celsius)

	BLE_UNIT_LUMINOUS_FLUX_LUMEN                                    = 0x2730, // luminous flux (lumen)
	BLE_UNIT_ILLUMINANCE_LUX                                        = 0x2731, // illuminance (lux)
	BLE_UNIT_ACTIVITY_REFERRED_TO_A_RADIONUCLIDE_BECQUEREL          = 0x2732, // activity referred to a radionuclide (becquerel)
	BLE_UNIT_ABSORBED_DOSE_GRAY                                     = 0x2733, // absorbed dose (gray)
	BLE_UNIT_DOSE_EQUIVALENT_SIEVERT                                = 0x2734, // dose equivalent (sievert)
	BLE_UNIT_CATALYTIC_ACTIVITY_KATAL                               = 0x2735, // catalytic activity (katal)

	BLE_UNIT_DYNAMIC_VISCOSITY_PASCAL_SECOND                        = 0x2740, // dynamic viscosity (pascal second)
	BLE_UNIT_MOMENT_OF_FORCE_NEWTON_METRE                           = 0x2741, // moment of force (newton metre)
	BLE_UNIT_SURFACE_TENSION_NEWTON_PER_METRE                       = 0x2742, // surface tension (newton per metre)
	BLE_UNIT_ANGULAR_VELOCITY_RADIAN_PER_SECOND                     = 0x2743, // angular velocity (radian per second)
	BLE_UNIT_ANGULAR_ACCELERATION_RADIAN_PER_SECOND_SQUARED         = 0x2744, // angular acceleration (radian per second squared)
	BLE_UNIT_HEAT_FLUX_DENSITY_WATT_PER_SQUARE_METRE                = 0x2745, // heat flux density (watt per square metre)
	BLE_UNIT_HEAT_CAPACITY_JOULE_PER_KELVIN                         = 0x2746, // heat capacity (joule per kelvin)
	BLE_UNIT_SPECIFIC_HEAT_CAPACITY_JOULE_PER_KILOGRAM_KELVIN       = 0x2747, // specific heat capacity (joule per kilogram kelvin)
	BLE_UNIT_SPECIFIC_ENERGY_JOULE_PER_KILOGRAM                     = 0x2748, // specific energy (joule per kilogram)
	BLE_UNIT_THERMAL_CONDUCTIVITY_WATT_PER_METRE_KELVIN             = 0x2749, // thermal conductivity (watt per metre kelvin)
	BLE_UNIT_ENERGY_DENSITY_JOULE_PER_CUBIC_METRE                   = 0x274A, // energy density (joule per cubic metre)
	BLE_UNIT_ELECTRIC_FIELD_STRENGTH_VOLT_PER_METRE                 = 0x274B, // electric field strength (volt per metre)
	BLE_UNIT_ELECTRIC_CHARGE_DENSITY_COULOMB_PER_CUBIC_METRE        = 0x274C, // electric charge density (coulomb per cubic metre)
	BLE_UNIT_SURFACE_CHARGE_DENSITY_COULOMB_PER_SQUARE_METRE        = 0x274D, // surface charge density (coulomb per square metre)
	BLE_UNIT_ELECTRIC_FLUX_DENSITY_COULOMB_PER_SQUARE_METRE         = 0x274E, // electric flux density (coulomb per square metre)
	BLE_UNIT_PERMITTIVITY_FARAD_PER_METRE                           = 0x274F, // permittivity (farad per metre)

	BLE_UNIT_PERMEABILITY_HENRY_PER_METRE                           = 0x2750, // permeability (henry per metre)
	BLE_UNIT_MOLAR_ENERGY_JOULE_PER_MOLE                            = 0x2751, // molar energy (joule per mole)
	BLE_UNIT_MOLAR_ENTROPY_JOULE_PER_MOLE_KELVIN                    = 0x2752, // molar entropy (joule per mole kelvin)
	BLE_UNIT_EXPOSURE_COULOMB_PER_KILOGRAM                          = 0x2753, // exposure (coulomb per kilogram)
	BLE_UNIT_ABSORBED_DOSE_RATE_GRAY_PER_SECOND                     = 0x2754, // absorbed dose rate (gray per second)
	BLE_UNIT_RADIANT_INTENSITY_WATT_PER_STERADIAN                   = 0x2755, // radiant intensity (watt per steradian)
	BLE_UNIT_RADIANCE_WATT_PER_SQUARE_METRE_STERADIAN               = 0x2756, // radiance (watt per square metre steradian)
	BLE_UNIT_CATALYTIC_ACTIVITY_CONCENTRATION_KATAL_PER_CUBIC_METRE = 0x2757, // catalytic activity concentration (katal per cubic metre)

	BLE_UNIT_TIME_MINUTE                                            = 0x2760, // time (minute)
	BLE_UNIT_TIME_HOUR                                              = 0x2761, // time (hour)
	BLE_UNIT_TIME_DAY                                               = 0x2762, // time (day)
	BLE_UNIT_PLANE_ANGLE_DEGREE                                     = 0x2763, // plane angle (degree)
	BLE_UNIT_PLANE_ANGLE_MINUTE                                     = 0x2764, // plane angle (minute)
	BLE_UNIT_PLANE_ANGLE_SECOND                                     = 0x2765, // plane angle (second)
	BLE_UNIT_AREA_HECTARE                                           = 0x2766, // area (hectare)
	BLE_UNIT_VOLUME_LITRE                                           = 0x2767, // volume (litre)
	BLE_UNIT_MASS_TONNE                                             = 0x2768, // mass (tonne)

	BLE_UNIT_PRESSURE_BAR                                           = 0x2780, // pressure (bar)
	BLE_UNIT_PRESSURE_MILLIMETRE_OF_MERCURY                         = 0x2781, // pressure (millimetre of mercury)
	BLE_UNIT_LENGTH_NGSTRM                                          = 0x2782, // length (ngstrm)
	BLE_UNIT_LENGTH_NAUTICAL_MILE                                   = 0x2783, // length (nautical mile)
	BLE_UNIT_AREA_BARN                                              = 0x2784, // area (barn)
	BLE_UNIT_VELOCITY_KNOT                                          = 0x2785, // velocity (knot)
	BLE_UNIT_LOGARITHMIC_RADIO_QUANTITY_NEPER                       = 0x2786, // logarithmic radio quantity (neper)
	BLE_UNIT_LOGARITHMIC_RADIO_QUANTITY_BEL                         = 0x2787, // logarithmic radio quantity (bel)

	BLE_UNIT_LENGTH_YARD                                            = 0x27A0, // length (yard)
	BLE_UNIT_LENGTH_PARSEC                                          = 0x27A1, // length (parsec)
	BLE_UNIT_LENGTH_INCH                                            = 0x27A2, // length (inch)
	BLE_UNIT_LENGTH_FOOT                                            = 0x27A3, // length (foot)
	BLE_UNIT_LENGTH_MILE                                            = 0x27A4, // length (mile)
	BLE_UNIT_PRESSURE_POUND_FORCE_PER_SQUARE_INCH                   = 0x27A5, // pressure (pound-force per square inch)
	BLE_UNIT_VELOCITY_KILOMETRE_PER_HOUR                            = 0x27A6, // velocity (kilometre per hour)
	BLE_UNIT_VELOCITY_MILE_PER_HOUR                                 = 0x27A7, // velocity (mile per hour)
	BLE_UNIT_ANGULAR_VELOCITY_REVOLUTION_PER_MINUTE                 = 0x27A8, // angular velocity (revolution per minute)
	BLE_UNIT_ENERGY_GRAM_CALORIE                                    = 0x27A9, // energy (gram calorie)
	BLE_UNIT_ENERGY_KILOGRAM_CALORIE                                = 0x27AA, // energy (kilogram calorie)
	BLE_UNIT_ENERGY_KILOWATT_HOUR                                   = 0x27AB, // energy (kilowatt hour)
	BLE_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_FAHRENHEIT            = 0x27AC, // thermodynamic temperature (degree Fahrenheit)
	BLE_UNIT_PERCENTAGE                                             = 0x27AD, // percentage
	BLE_UNIT_PER_MILLE                                              = 0x27AE, // per mille
	BLE_UNIT_PERIOD_BEATS_PER_MINUTE                                = 0x27AF, // period (beats per minute)

	BLE_UNIT_ELECTRIC_CHARGE_AMPERE_HOURS                           = 0x27B0, // electric charge (ampere hours)
	BLE_UNIT_MASS_DENSITY_MILLIGRAM_PER_DECILITRE                   = 0x27B1, // mass density (milligram per decilitre)
	BLE_UNIT_MASS_DENSITY_MILLIMOLE_PER_LITRE                       = 0x27B2, // mass density (millimole per litre)
	BLE_UNIT_TIME_YEAR                                              = 0x27B3, // time (year)
	BLE_UNIT_TIME_MONTH                                             = 0x27B4, // time (month)
	BLE_UNIT_CONCENTRATION_COUNT_PER_CUBIC_METRE                    = 0x27B5, // concentration (count per cubic metre)
	BLE_UNIT_IRRADIANCE_WATT_PER_SQUARE_METRE                       = 0x27B6, // irradiance (watt per square metre)
	BLE_UNIT_TRANSFER_RATE_MILLILITER_PER_KILOGRAM_PER_MINUTE       = 0x27B7, // milliliter (per kilogram per minute)
	BLE_UNIT_MASS_POUND                                             = 0x27B8, // mass (pound)
	BLE_UNIT_METABOLIC_EQUIVALENT                                   = 0x27B9, // metabolic equivalent
	BLE_UNIT_STEP_PER_MINUTE                                        = 0x27BA, // step (per minute)
	BLE_UNIT_STROKE_PER_MINUTE                                      = 0x27BC, // stroke (per minute)
	BLE_UNIT_VELOCITY_KILOMETER_PER_MINUTE                          = 0x27BD, // pace (kilometre per minute)
	BLE_UNIT_LUMINOUS_EFFICACY_LUMEN_PER_WATT                       = 0x27BE, // luminous efficacy (lumen per watt)
	BLE_UNIT_LUMINOUS_ENERGY_LUMEN_HOUR                             = 0x27BF, // luminous energy (lumen hour)

	BLE_UNIT_LUMINOUS_EXPOSURE_LUX_HOUR                             = 0x27C0, // luminous exposure (lux hour)
	BLE_UNIT_MASS_FLOW_GRAM_PER_SECOND                              = 0x27C1, // mass flow (gram per second)
	BLE_UNIT_VOLUME_FLOW_LITRE_PER_SECOND                           = 0x27C2  // volume flow (litre per second)
};

void sunrise_ble_init(void);

#endif /* SUNRISE_BLE_H */
