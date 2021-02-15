#include "error.h"

#include <gks/gks.h>
#include <gks/gkserror.h>

#include <catch2/catch.hpp>

TEST_CASE("GKS closed", "[errors]")
{
    g_recordedErrors.clear();
    REQUIRE(getGksOpState() == GGKCL);

    SECTION("gclosegks")
    {
        gclosegks();

        requireError(GERROR_NOT_STATE_GKOP, GFN_CLOSE_GKS);
    }
    SECTION("gopenws")
    {
        Gint wsId{1};
        const Gchar *connId{"tek4105"};
        Gint wsType{};
        gopenws(wsId, connId, wsType);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_OPEN_WORKSTATION);
    }
    SECTION("gclosews")
    {
        Gint wsId{1};
        gclosews(wsId);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_CLOSE_WORKSTATION);
    }
    SECTION("GKS value inquiry")
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
    SECTION("gsetlinetype")
    {
        gsetlinetype(0);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_LINETYPE);
    }
    SECTION("gsetlinecolorind")
    {
        gsetlinecolorind(0);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_POLYLINE_COLOR_INDEX);
    }
    SECTION("gsetmarkertype")
    {
        gsetmarkertype(0);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_MARKER_TYPE);
    }
    SECTION("gsetmarkercolorind")
    {
        gsetmarkercolorind(0);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_POLYMARKER_COLOR_INDEX);
    }
    SECTION("gsettextcolorind")
    {
        gsettextcolorind(0);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_TEXT_COLOR_INDEX);
    }
    SECTION("gsetcharheight")
    {
        gsetcharheight(0.0f);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_CHARACTER_HEIGHT);
    }
    SECTION("gsetcharheight")
    {
        gsetcharup(nullptr);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_CHARACTER_UP_VECTOR);
    }
    SECTION("gsettextalign")
    {
        gsettextalign(nullptr);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_TEXT_ALIGNMENT);
    }
    SECTION("gsetfillstyle")
    {
        gsetfillstyle(GHOLLOW);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_FILL_AREA_INTERIOR_STYLE);
    }
    SECTION("gsetfillcolorind")
    {
        gsetfillcolorind(0);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_FILL_AREA_COLOR_INDEX);
    }
    SECTION("gsetwindow")
    {
        gsetwindow(0, nullptr);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_WINDOW);
    }
    SECTION("gsetviewport")
    {
        gsetviewport(0, nullptr);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_VIEWPORT);
    }
    SECTION("gselntran")
    {
        gselntran(0);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SELECT_NORMALIZATION_TRANSFORMATION);
    }
    SECTION("gsetcolorrep")
    {
        Gint wsId{1};
        Gint index{1};
        Gcobundl color{1.0f, 1.0f, 1.0f};
        gsetcolorrep(wsId, index, &color);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_COLOR_REPRESENTATION);
    }
    SECTION("gsetwswindow")
    {
        gsetwswindow(0, nullptr);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_WORKSTATION_WINDOW);
    }
    SECTION("gsetwsviewport")
    {
        gsetwsviewport(0, nullptr);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_WORKSTATION_VIEWPORT);
    }
    SECTION("gactivatews")
    {
        Gint wsId{1};
        gactivatews(wsId);

        requireError(GERROR_NOT_STATE_WSOP_WSAC, GFN_ACTIVATE_WORKSTATION);
    }
    SECTION("gdeactivatews")
    {
        Gint wsId{1};
        gdeactivatews(wsId);

        requireError(GERROR_NOT_STATE_WSAC, GFN_DEACTIVATE_WORKSTATION);
    }
    SECTION("gclearws")
    {
        Gint wsId{1};
        gclearws(wsId, GALWAYS);

        requireError(GERROR_NOT_STATE_WSOP_WSAC, GFN_CLEAR_WORKSTATION);
    }
    SECTION("gupdatews")
    {
        Gint wsId{1};
        gupdatews(wsId, GPERFORM);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_UPDATE_WORKSTATION);
    }
    SECTION("gescape")
    {
        gescape(0, nullptr, 0, nullptr, nullptr);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_ESCAPE);
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

    REQUIRE(getGksOpState() == GGKCL);
}
