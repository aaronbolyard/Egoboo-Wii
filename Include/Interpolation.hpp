// This file is part of Egoboo-Wii.
//
// Egoboo-Wii is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Egoboo-Wii is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Egoboo-Wii.  If not, see <http:// www.gnu.org/licenses/>.

#ifndef ADVENTURE_INTERPOLATION_HPP_
#define ADVENTURE_INTERPOLATION_HPP_

namespace Adventure
{
	namespace Math
	{
		template<typename TType>
		inline TType QuadraticInterpolate(TType p1, TType p2, TType p3, float mu)
		{
			float inverseMu = (1.0f - mu);
			
			return (p1 * inverseMu * inverseMu) + (p2 * 2.0f * inverseMu * mu) + (p3 * mu * mu);
		}
		
		template<typename TType>
		inline TType LinearInterpolate(TType from, TType to, float mu)
		{
			return from + (to - from) * mu;
		}
	}
}

#endif
