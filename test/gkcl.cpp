#include "error.h"

#include <gks/gks.h>
#include <gks/gkserror.h>

#include <catch.hpp>

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
            status = ginqasf(&value);
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
            status = ginqavailwstypes(numWsTypes, start, &wsTypes, &numAvailTypes);
        }
        SECTION("ginqcharexpan")
        {
            Gfloat value{};
            status = ginqcharexpan(&value);
        }
        SECTION("ginqcharbase")
        {
            Gpoint value{};
            status = ginqcharbase(&value);
        }
        SECTION("ginqcharheight")
        {
            Gfloat value{};
            status = ginqcharheight(&value);
        }
        SECTION("ginqcharspace")
        {
            Gfloat value{};
            status = ginqcharspace(&value);
        }
        SECTION("ginqcharwidth")
        {
            Gfloat value{};
            status = ginqcharwidth(&value);
        }
        SECTION("ginqcharup")
        {
            Gpoint value{};
            status = ginqcharup(&value);
        }
        SECTION("ginqclip")
        {
            Gcliprect clipping{};
            status = ginqclip(&clipping);
        }
        SECTION("ginqcurrntrannum")
        {
            Gint transform{-1};
            status = ginqcurrntrannum(&transform);
        }
        SECTION("ginqfillcolorind")
        {
            Gint value{};
            status = ginqfillcolorind(&value);
        }
        SECTION("ginqfillind")
        {
            Gint value{};
            status = ginqfillind(&value);
        }
        SECTION("ginqfillstyle")
        {
            Gflinter value{};
            status = ginqfillstyle(&value);
        }
        SECTION("ginqfillstyleind")
        {
            Gint value{};
            status = ginqfillstyleind(&value);
        }
        SECTION("ginqlevelgks")
        {
            Glevel level{};
            status = ginqlevelgks(&level);
        }
        SECTION("ginqlinecolorind")
        {
            Gint value{};
            status = ginqlinecolorind(&value);
        }
        SECTION("ginqlineind")
        {
            Gint value{};
            status = ginqlineind(&value);
        }
        SECTION("ginqlinetype")
        {
            Gint value{};
            status = ginqlinetype(&value);
        }
        SECTION("ginqmarkercolorind")
        {
            Gint value{};
            status = ginqmarkercolorind(&value);
        }
        SECTION("ginqmarkerind")
        {
            Gint value{};
            status = ginqmarkerind(&value);
        }
        SECTION("ginqmarkersize")
        {
            Gfloat value{};
            status = ginqmarkersize(&value);
        }
        SECTION("ginqmarkertype")
        {
            Gint value{};
            status = ginqmarkertype(&value);
        }
        SECTION("ginqmaxntrannum")
        {
            Gint value{};
            status = ginqmaxntrannum(&value);
        }
        SECTION("ginqntran")
        {
            Gtran value{};
            status = ginqntran(0, &value);
        }
        SECTION("ginqopenws")
        {
            std::vector<Gint> ids(10);
            Gintlist idList{static_cast<Gint>(ids.size()), ids.data()};
            Gint start{0};
            Gint numIds{0};
            status = ginqopenws(idList.number, start, &idList, &numIds);
        }
        SECTION("ginqpatheight")
        {
            Gpoint value{};
            status = ginqpatheight(&value);
        }
        SECTION("ginqpatrefpt")
        {
            Gpoint refPt{};
            status = ginqpatrefpt(&refPt);
        }
        SECTION("ginqpatwidth")
        {
            Gpoint value{};
            status = ginqpatwidth(&value);
        }
        SECTION("ginqtextalign")
        {
            Gtxalign value{};
            status = ginqtextalign(&value);
        }
        SECTION("ginqtextcolorind")
        {
            Gint value{};
            status = ginqtextcolorind(&value);
        }
        SECTION("ginqtextfontprec")
        {
            Gtxfp value{};
            status = ginqtextfontprec(&value);
        }
        SECTION("ginqtextind")
        {
            Gint value{};
            status = ginqtextind(&value);
        }
        SECTION("ginqcolorfacil")
        {
            Gcofac facil{};
            status = ginqcolorfacil(GWSTYPE_TEK4105, 0, NULL, &facil);
        }
        SECTION("ginqdisplaysize")
        {
            Gdspsize value{};
            status = ginqdisplaysize(GWSTYPE_TEK4105, &value);
        }
        SECTION("ginqfillfacil")
        {
            Gflinter interiorStyles[4]{};
            Gint hatchStyles[10]{};
            Gflfac facil{};
            facil.interiors = interiorStyles;
            facil.hatches = hatchStyles;
            Gint numAvailHatches{};
            status = ginqfillfacil(GWSTYPE_TEK4105, numOf(hatchStyles), &numAvailHatches, &facil);
        }
        SECTION("ginqlinefacil")
        {
            Gint lineTypes[10]{};
            Gint numLineTypes{};
            Glnfac value{};
            status = ginqlinefacil(GWSTYPE_TEK4105, numOf(lineTypes), &numLineTypes, &value);
        }
        SECTION("ginqmarkerfacil")
        {
            Gint lineTypes[10]{};
            Gint numLineTypes{};
            Glnfac value{};
            status = ginqlinefacil(GWSTYPE_TEK4105, numOf(lineTypes), &numLineTypes, &value);
        }
        SECTION("ginqmaxwssttables")
        {
            Gwstables tables{-1, -1, -1, -1, -1, -1};
            status = ginqmaxwssttables(GWSTYPE_TEK4105, &tables);
        }

        SECTION("ginqwsmaxnum")
        {
            Gwsmax value{};
            status = ginqwsmaxnum(&value);
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
        Gpoint up{0.0f, 1.0f};
        gsetcharup(&up);

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

        requireGksOpen(GFN_SET_FILL_AREA_INDEX);
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
        Glimit viewport{0.0f, 1.0f, 0.0f, 1.0f};
        gsetviewport(1, &viewport);

        requireGksOpen(GFN_SET_VIEWPORT);
    }
    SECTION("gsetwindow")
    {
        Glimit window{0.0f, 1.0f, 0.0f, 1.0f};
        gsetwindow(1, &window);

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
        Glimit value{0.0f, 1.0f, 0.0f, 1.0f};
        gsetwswindow(0, &value);

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

    SECTION("inquire")
    {
        Gint wsId{0};
        Gint status{};

        SECTION("ginqcolorindices")
        {
            Gint index{1};
            Gcobundl value{};
            status = ginqcolorrep(wsId, index, &value);

            REQUIRE(status == GERROR_NOT_STATE_WSOP_WSAC_SGOP);
        }
        SECTION("ginqcolorrep")
        {
            Gint index{1};
            Gcobundl value{};
            status = ginqcolorrep(wsId, index, &value);

            REQUIRE(status == GERROR_NOT_STATE_WSOP_WSAC_SGOP);
        }
        SECTION("ginqtextextent")
        {
            Gpoint pos{};
            Gextent value{};
            status = ginqtextextent(wsId, &pos, "Hello, world!", &value);

            REQUIRE(status == GERROR_NOT_STATE_WSOP_WSAC_SGOP);
        }
        SECTION("ginqwsconntype")
        {
            Gwsct ct{};
            Gint buffSize{};
            Gint numCts{};
            status = ginqwsconntype(wsId + 1, buffSize, &numCts, &ct);

            REQUIRE(status == GERROR_NOT_STATE_WSOP_WSAC_SGOP);
        }
        SECTION("ginqwstran")
        {
            Gwsti transform{};
            status = ginqwstran(wsId, &transform);

            REQUIRE(status == GERROR_NOT_STATE_WSOP_WSAC_SGOP);
        }
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
        Grect rect =
        {
            {0.25f, 0.75f},
            {0.75f, 0.25f}
        };
        Gidim dim =
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
