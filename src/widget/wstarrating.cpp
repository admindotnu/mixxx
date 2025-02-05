#include "widget/wstarrating.h"

#include <QApplication>
#include <QSize>
#include <QStyleOption>
#include <QStylePainter>

#include "moc_wstarrating.cpp"
#include "track/track.h"

WStarRating::WStarRating(const QString& group, QWidget* pParent)
        : WWidget(pParent),
          m_starRating(0, 5),
          m_group(group),
          m_focused(false) {
    // Controls to change the star rating with controllers.
    // Note that 'group' maybe NULLPTR, e.g. when called from DlgTrackInfo,
    // so only create rate change COs if there's a group passed when creating deck widgets.
    if (!m_group.isEmpty()) {
        m_pStarsUp = std::make_unique<ControlPushButton>(ConfigKey(group, "stars_up"));
        m_pStarsDown = std::make_unique<ControlPushButton>(ConfigKey(group, "stars_down"));
        connect(m_pStarsUp.get(), &ControlObject::valueChanged, this, &WStarRating::slotStarsUp);
        connect(m_pStarsDown.get(),
                &ControlObject::valueChanged,
                this,
                &WStarRating::slotStarsDown);
    }
}

void WStarRating::setup(const QDomNode& node, const SkinContext& context) {
    Q_UNUSED(node);
    Q_UNUSED(context);
    setMouseTracking(true);
    setFocusPolicy(Qt::NoFocus);
}

QSize WStarRating::sizeHint() const {
    QStyleOption option;
    option.initFrom(this);
    QSize widgetSize = style()->sizeFromContents(QStyle::CT_PushButton, &option,
                                                 m_starRating.sizeHint(), this);

    m_contentRect.setRect(
        (widgetSize.width() - m_starRating.sizeHint().width()) / 2,
        (widgetSize.height() - m_starRating.sizeHint().height()) / 2,
        m_starRating.sizeHint().width(),
        m_starRating.sizeHint().height()
    );

    return widgetSize;
}

void WStarRating::slotTrackLoaded(TrackPointer pTrack) {
    if (m_pCurrentTrack != pTrack) {
        if (m_pCurrentTrack) {
            disconnect(m_pCurrentTrack.get(), nullptr, this, nullptr);
            m_pCurrentTrack.reset();
        }
        if (pTrack) {
            connect(pTrack.get(),
                    &Track::changed,
                    this,
                    &WStarRating::slotTrackChanged);
            m_pCurrentTrack = pTrack;
        }
        updateRatingFromTrack();
    }
}

void WStarRating::setRating(int rating) {
    // Check consistency with the connected track
    DEBUG_ASSERT(!m_pCurrentTrack ||
            m_pCurrentTrack->getRating() == rating);
    m_starRating.setStarCount(rating);
    update();
}

void WStarRating::updateRatingFromTrack() {
    if (m_pCurrentTrack) {
        setRating(m_pCurrentTrack->getRating());
    } else {
        setRating(0);
    }
}

void WStarRating::slotTrackChanged(TrackId trackId) {
    Q_UNUSED(trackId);
    updateRatingFromTrack();
}

void WStarRating::paintEvent(QPaintEvent * /*unused*/) {
    QStyleOption option;
    option.initFrom(this);
    QStylePainter painter(this);

    painter.setBrush(option.palette.text());
    painter.drawPrimitive(QStyle::PE_Widget, option);

    m_starRating.paint(&painter, m_contentRect);
}

void WStarRating::mouseMoveEvent(QMouseEvent *event) {
    if (!m_pCurrentTrack) {
        return;
    }

    m_focused = true;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    int star = starAtPosition(event->position().x());
#else
    int star = starAtPosition(event->x());
#endif

    if (star != m_starRating.starCount() && star != -1) {
        m_starRating.setStarCount(star);
        update();
    }
}

void WStarRating::slotStarsUp(double v) {
    if (!m_pCurrentTrack) {
        return;
    }
    if (v > 0 && m_starRating.starCount() < m_starRating.maxStarCount()) {
        int star = m_starRating.starCount() + 1;
        m_starRating.setStarCount(star);
        update();
        m_pCurrentTrack->setRating(star);
    }
}

void WStarRating::slotStarsDown(double v) {
    if (!m_pCurrentTrack) {
        return;
    }
    if (v > 0 && m_starRating.starCount() > 0) {
        int star = m_starRating.starCount() - 1;
        m_starRating.setStarCount(star);
        update();
        m_pCurrentTrack->setRating(star);
    }
}

void WStarRating::leaveEvent(QEvent* /*unused*/) {
    m_focused = false;
    updateRatingFromTrack();
}

// The method uses basic linear algebra to find out which star is under the cursor.
int WStarRating::starAtPosition(int x) {
    // If the mouse is very close to the left edge, set 0 stars.
    if (x < m_starRating.sizeHint().width() * 0.05) {
        return 0;
    }
    int star = (x / (m_starRating.sizeHint().width() / m_starRating.maxStarCount())) + 1;

    if (star <= 0 || star > m_starRating.maxStarCount()) {
        return 0;
    }

    return star;
}

void WStarRating::mouseReleaseEvent(QMouseEvent* /*unused*/) {
    if (!m_pCurrentTrack) {
        return;
    }

    m_pCurrentTrack->setRating(m_starRating.starCount());
}

void WStarRating::fillDebugTooltip(QStringList* debug) {
    WWidget::fillDebugTooltip(debug);

    QString currentRating = "-";
    QString maximumRating = QString::number(m_starRating.maxStarCount());

    if (m_pCurrentTrack) {
        currentRating.setNum(m_pCurrentTrack->getRating());
    }

    *debug << QString("Rating: %1/%2").arg(currentRating, maximumRating);
}
