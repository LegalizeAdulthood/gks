#include "error.h"

#include <gks/gks.h>
#include <gks/gkserror.h>

#include <catch2/catch.hpp>

namespace
{

inline void requireGksOpen(GFunction fn)
{
    requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, fn);
}

}

TEST_CASE("GKS closed", "[errors]")
{
    g_recordedErrors.clear();
    REQUIRE(getGksOpState() == GGKCL);

    SECTION("gclosegks")
    {
        gclosegks();

        requireError(GERROR_NOT_STATE_GKOP, GFN_CLOSE_GKS);
    }

    SECTION("value inquiry")
    {
        Gint status{};

        SECTION("ginqasf")
        {
            Gasfs value{};
            ginqasf(&value, &status);
        }
        SECTION("ginqavailwstypes")
        {
            const Gint numWsTypes{10};
            Gint start{};
            int wsTypeValues[numWsTypes];
            for (int &wsTypeValue : wsTypeValues)
            {
                wsTypeValue = -1;
            }
            Gintlist wsTypes{0, wsTypeValues};
            Gint numAvailTypes{};
            ginqavailwstypes(numWsTypes, start, &wsTypes, &numAvailTypes, &status);
        }
        SECTION("ginqcharexpan")
        {
            Gfloat value{};
            ginqcharexpan(&value, &status);
        }
        SECTION("ginqcharbase")
        {
            Gpoint value{};
            ginqcharbase(&value, &status);
        }
        SECTION("ginqcharheight")
        {
            Gfloat value{};
            ginqcharheight(&value, &status);
        }
        SECTION("ginqcharspace")
        {
            Gfloat value{};
            ginqcharspace(&value, &status);
        }
        SECTION("ginqcharwidth")
        {
            Gfloat value{};
            ginqcharwidth(&value, &status);
        }
        SECTION("ginqcharup")
        {
            Gpoint value{};
            ginqcharup(&value, &status);
        }
        SECTION("ginqclip")
        {
            Gcliprect clipping{};
            ginqclip(&clipping, &status);
        }
        SECTION("ginqcurrntrannum")
        {
            Gint transform{-1};
            ginqcurrntrannum(&transform, &status);
        }
        SECTION("ginqfillcolorind")
        {
            Gint value{};
            ginqfillcolorind(&value, &status);
        }
        SECTION("ginqfillind")
        {
            Gint value{};
            ginqfillind(&value, &status);
        }
        SECTION("ginqfillstyle")
        {
            Gflinter value{};
            ginqfillstyle(&value, &status);
        }
        SECTION("ginqfillstyleind")
        {
            Gint value{};
            ginqfillstyleind(&value, &status);
        }
        SECTION("ginqlevelgks")
        {
            Glevel level{};
            ginqlevelgks(&level, &status);
        }
        SECTION("ginqlinecolorind")
        {
            Gint value{};
            ginqlinecolorind(&value, &status);
        }
        SECTION("ginqlineind")
        {
            Gint value{};
            ginqlineind(&value, &status);
        }
        SECTION("ginqlinetype")
        {
            Gint value{};
            ginqlinetype(&value, &status);
        }
        SECTION("ginqmarkercolorind")
        {
            Gint value{};
            ginqmarkercolorind(&value, &status);
        }
        SECTION("ginqmarkerind")
        {
            Gint value{};
            ginqmarkerind(&value, &status);
        }
        SECTION("ginqmarkersize")
        {
            Gfloat value{};
            ginqmarkersize(&value, &status);
        }
        SECTION("ginqmarkertype")
        {
            Gint value{};
            ginqmarkertype(&value, &status);
        }
        SECTION("ginqmaxntrannum")
        {
            Gint value{};
            ginqmaxntrannum(&value, &status);
        }
        SECTION("ginqntran")
        {
            Gtran value{};
            ginqntran(0, &value, &status);
        }
        SECTION("ginqopenws")
        {
            std::vector<Gint> ids(10);
            Gintlist idList{static_cast<Gint>(ids.size()), ids.data()};
            Gint start{0};
            Gint numIds{0};
            ginqopenws(idList.number, start, &idList, &numIds, &status);
        }
        SECTION("ginqpatrefpt")
        {
            Gpoint refPt{};
            ginqpatrefpt(&refPt, &status);
        }
        SECTION("ginqpatsize")
        {
            Gpoint value{};
            ginqpatsize(&value, &status);
        }
        SECTION("ginqtextalign")
        {
            Gtxalign value{};
            ginqtextalign(&value, &status);
        }
        SECTION("ginqtextcolorind")
        {
            Gint value{};
            ginqtextcolorind(&value, &status);
        }
        SECTION("ginqtextfontprec")
        {
            Gtxfp value{};
            ginqtextfontprec(&value, &status);
        }
        SECTION("ginqtextind")
        {
            Gint value{};
            ginqtextind(&value, &status);
        }
        SECTION("ginqcolorfacil")
        {
            Gcofac facil{};
            ginqcolorfacil(GWSTYPE_TEK4105, 0, NULL, &facil, &status);
        }
        SECTION("ginqdisplaysize")
        {
            Gdspsize value{};
            ginqdisplaysize(GWSTYPE_TEK4105, &value, &status);
        }
        SECTION("ginqfillfacil")
        {
            Gflinter interiorStyles[4]{};
            Gint hatchStyles[10]{};
            Gflfac facil{};
            facil.interiors = interiorStyles;
            facil.hatches = hatchStyles;
            Gint numAvailHatches{};
            ginqfillfacil(GWSTYPE_TEK4105, numOf(hatchStyles), &numAvailHatches, &facil, &status);
        }
        SECTION("ginqlinefacil")
        {
            Gint lineTypes[10]{};
            Gint numLineTypes{};
            Glnfac value{};
            ginqlinefacil(GWSTYPE_TEK4105, numOf(lineTypes), &numLineTypes, &value, &status);
        }
        SECTION("ginqwsmaxnum")
        {
            Gwsmax value{};
            ginqwsmaxnum(&value, &status);
        }

        REQUIRE(status == GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP);
    }

    SECTION("gselntran")
    {
        gselntran(0);

        requireGksOpen(GFN_SELECT_NORMALIZATION_TRANSFORMATION);
    }
    SECTION("gsetasf")
    {
        Gasfs value{};
        gsetasf(&value);

        requireGksOpen(GFN_SET_ASPECT_SOURCE_FLAGS);
    }
    SECTION("gsetcharexpan")
    {
        gsetcharexpan(1.0f);

        requireGksOpen(GFN_SET_CHARACTER_EXPANSION_FACTOR);
    }
    SECTION("gsetcharheight")
    {
        gsetcharheight(0.02f);

        requireGksOpen(GFN_SET_CHARACTER_HEIGHT);
    }
    SECTION("gsetcharspace")
    {
        gsetcharspace(1.0f);

        requireGksOpen(GFN_SET_CHARACTER_SPACING);
    }
    SECTION("gsetcharup")
    {
        gsetcharup(nullptr);

        requireGksOpen(GFN_SET_CHARACTER_UP_VECTOR);
    }
    SECTION("gsetclip")
    {
        gsetclip(GCLIP);

        requireGksOpen(GFN_SET_CLIPPING_INDICATOR);
    }
    SECTION("gsetfillcolorind")
    {
        gsetfillcolorind(0);

        requireGksOpen(GFN_SET_FILL_AREA_COLOR_INDEX);
    }
    SECTION("gsetfillind")
    {
        gsetfillind(1);

        requireGksOpen(GFN_SET_FILL_AREA_COLOR_INDEX);
    }
    SECTION("gsetfillstyle")
    {
        gsetfillstyle(GHOLLOW);

        requireGksOpen(GFN_SET_FILL_AREA_INTERIOR_STYLE);
    }
    SECTION("gsetfillstyleind")
    {
        gsetfillstyleind(1);

        requireGksOpen(GFN_SET_FILL_AREA_INTERIOR_STYLE);
    }
    SECTION("gsetlinecolorind")
    {
        gsetlinecolorind(0);

        requireGksOpen(GFN_SET_POLYLINE_COLOR_INDEX);
    }
    SECTION("gsetlineind")
    {
        gsetlineind(1);

        requireGksOpen(GFN_SET_POLYLINE_INDEX);
    }
    SECTION("gsetlinetype")
    {
        gsetlinetype(1);

        requireGksOpen(GFN_SET_LINETYPE);
    }
    SECTION("gsetmarkercolorind")
    {
        gsetmarkercolorind(0);

        requireGksOpen(GFN_SET_POLYMARKER_COLOR_INDEX);
    }
    SECTION("gsetmarkerind")
    {
        gsetmarkerind(1);

        requireGksOpen(GFN_SET_POLYMARKER_INDEX);
    }
    SECTION("gsetmarkersize")
    {
        gsetmarkersize(1.0f);

        requireGksOpen(GFN_SET_MARKER_SIZE_SCALE_FACTOR);
    }
    SECTION("gsetmarkertype")
    {
        gsetmarkertype(GMK_DIAGONAL_CROSS);

        requireGksOpen(GFN_SET_MARKER_TYPE);
    }
    SECTION("gsettextalign")
    {
        gsettextalign(nullptr);

        requireGksOpen(GFN_SET_TEXT_ALIGNMENT);
    }
    SECTION("gsettextcolorind")
    {
        gsettextcolorind(0);

        requireGksOpen(GFN_SET_TEXT_COLOR_INDEX);
    }
    SECTION("gsettextfontprec")
    {
        Gtxfp value{1, GP_STRING};
        gsettextfontprec(&value);

        requireGksOpen(GFN_SET_TEXT_FONT_AND_PRECISION);
    }
    SECTION("gsettextind")
    {
        gsettextind(1);

        requireGksOpen(GFN_SET_TEXT_INDEX);
    }
    SECTION("gsetviewport")
    {
        gsetviewport(0, nullptr);

        requireGksOpen(GFN_SET_VIEWPORT);
    }
    SECTION("gsetwindow")
    {
        gsetwindow(0, nullptr);

        requireGksOpen(GFN_SET_WINDOW);
    }

    SECTION("gopenws")
    {
        Gint wsId{0};
        const Gchar *connId{"tek4105"};
        Gint wsType{GWSTYPE_TEK4105};
        gopenws(wsId, connId, wsType);

        requireGksOpen(GFN_OPEN_WORKSTATION);
    }
    SECTION("gclosews")
    {
        Gint wsId{0};
        gclosews(wsId);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_CLOSE_WORKSTATION);
    }

    SECTION("gsetwsviewport")
    {
        gsetwsviewport(0, nullptr);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_WORKSTATION_VIEWPORT);
    }
    SECTION("gsetwswindow")
    {
        gsetwswindow(0, nullptr);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_WORKSTATION_WINDOW);
    }

    SECTION("gactivatews")
    {
        Gint wsId{0};
        gactivatews(wsId);

        requireError(GERROR_NOT_STATE_WSOP_WSAC, GFN_ACTIVATE_WORKSTATION);
    }
    SECTION("gclearws")
    {
        Gint wsId{0};
        gclearws(wsId, GALWAYS);

        requireError(GERROR_NOT_STATE_WSOP_WSAC, GFN_CLEAR_WORKSTATION);
    }
    SECTION("gdeactivatews")
    {
        Gint wsId{0};
        gdeactivatews(wsId);

        requireError(GERROR_NOT_STATE_WSAC, GFN_DEACTIVATE_WORKSTATION);
    }
    SECTION("gescape")
    {
        gescape(0, nullptr, 0, nullptr, nullptr);

        requireGksOpen(GFN_ESCAPE);
    }
    SECTION("gupdatews")
    {
        Gint wsId{0};
        gupdatews(wsId, GPERFORM);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_UPDATE_WORKSTATION);
    }

    SECTION("ginqcolorrep")
    {
        Gint wsId{0};
        Gint index{1};
        Gcobundl value{};
        Gint status{};
        ginqcolorrep(wsId, index, &value, &status);

        REQUIRE(status == GERROR_NOT_STATE_WSOP_WSAC_SGOP);
    }
    SECTION("ginqwstran")
    {
        Gint wsId{0};
        struct Gwsti transform{};
        Gint status{-1};
        ginqwstran(wsId, &transform, &status);

        REQUIRE(status == GERROR_NOT_STATE_WSOP_WSAC_SGOP);
    }

    SECTION("gsetcolorrep")
    {
        Gint wsId{0};
        Gint index{1};
        Gcobundl color{1.0f, 1.0f, 1.0f};
        gsetcolorrep(wsId, index, &color);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_COLOR_REPRESENTATION);
    }

    SECTION("gcellarray")
    {
        struct Grect rect =
        {
            {0.25f, 0.75f},
            {0.75f, 0.25f}
        };
        struct Gidim dim =
        {
            128, 128
        };
        Gint colors[] = { 0, 1 };
        gcellarray(&rect, &dim, colors);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_CELL_ARRAY);
    }
    SECTION("gfillarea")
    {
        gfillarea(0, nullptr);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_FILL_AREA);
    }
    SECTION("ggdp")
    {
        ggdp(0, nullptr, 0, nullptr);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_GDP);
    }
    SECTION("gpolyline")
    {
        gpolyline(0, nullptr);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYLINE);
    }
    SECTION("gpolymarker")
    {
        gpolymarker(0, nullptr);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYMARKER);
    }
    SECTION("gtext")
    {
        gtext(nullptr, nullptr);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_TEXT);
    }

    REQUIRE(getGksOpState() == GGKCL);
}
