import React from 'react';

function EndReport(props) {

    const report = (subject) => {
        if(subject) {
            return (
                <div className="end-text">
                    <p>{subject}</p>
                </div>
            );
        }
    }

    const renderReport = () => {
        if(props.endState) {
            return (
                <div>
                        {report(props.endState.condition)}
                        {report(props.pieceColor(props.endState.winner))}
                </div>
            );
        }
    }

    return (
        <div className="end-report">
            {renderReport()}
        </div>
    );
}

export default EndReport;

