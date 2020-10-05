/*--------------------------------------------------------------------------------- */
/* AdapterHandler.hpp - dgVoodooSetup general video adapter handling interface      */
/*                                                                                  */
/* Copyright (C) 2005 Dege                                                          */
/*                                                                                  */
/* This library is free software; you can redistribute it and/or                    */
/* modify it under the terms of the GNU Lesser General Public                       */
/* License as published by the Free Software Foundation; either                     */
/* version 2.1 of the License, or (at your option) any later version.               */
/*                                                                                  */
/* This library is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of                   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                */
/* Lesser General Public License for more details.                                  */
/*                                                                                  */
/* You should have received a copy of the GNU Lesser General Public                 */
/* License along with this library; if not, write to the Free Software              */
/* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA   */
/*--------------------------------------------------------------------------------- */

class	IAdapterHandler
{
public:
	class	IResolutionEnumCallback
	{
	public:
		virtual void	AddResolution (unsigned int xRes, unsigned int yRes) = 0;
	};

	class	IRefreshRateEnumCallback
	{
	public:
		virtual void	AddRefreshRate (unsigned int freq) = 0;
	};

public:
	virtual const char*		GetApiName () = 0;

	virtual	bool			Init () = 0;
	virtual	void			ShutDown () = 0;
	
	virtual	unsigned int	GetNumOfAdapters () = 0;
	virtual	const char*		GetDescriptionOfAdapter (unsigned int index) = 0;

	virtual	unsigned int	GetNumOfDeviceTypes (unsigned int adapter) = 0;
	virtual	const char*		GetDescriptionOfDeviceType (unsigned int adapter, unsigned int index) = 0;

	virtual	void			EnumResolutions (unsigned int adapter, IResolutionEnumCallback& callback) = 0;
	virtual	void			EnumRefreshRates (unsigned int adapter, unsigned int xRes, unsigned int yRes,
											  unsigned int bitDepth, IRefreshRateEnumCallback& callback) = 0;

	virtual bool			GetCurrentDisplayMode (unsigned int& xRes, unsigned int& yRes,
												   unsigned int& bitDepth, unsigned int& refRate) = 0;
};

extern	IAdapterHandler*	GetDX7AdapterHandler ();
extern	IAdapterHandler*	GetDX9AdapterHandler ();
